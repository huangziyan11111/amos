// $Id$

// This program reads a set of contigs from a bank then reports 
// regions where mate-pair information indicates a problem

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <getopt.h>
#include <map>
#include <set>
#include <list>
#include <vector>
#include <string>
#include <math.h>
#include <functional>
#include "foundation_AMOS.hh"

using namespace std;
using namespace AMOS;
using namespace HASHMAP;

enum MateStatus {MP_GOOD, MP_SHORT, MP_LONG, MP_NORMAL, MP_OUTIE};

class AnnotatedMatePair: public Matepair_t
{
public:
  MateStatus status;
  Pos_t deviation;    // how far it deviates from mean
  AnnotatedMatePair() : Matepair_t() {
    status = MP_GOOD;
    deviation = 0;
  }
};


map<string, string> globals; // global variables

void printHelpText()
{
  cerr << 
    "\n"
    ".NAME.\n"
    "asmQC - computes several mate-pair based statistics on an assembly\n"
    "\n.USAGE.\n"
    "asmQC -b[ank] <bank_name>\n"
    "\n.OPTIONS.\n"
    "-h, -help     print out help message\n"
    "-b, -bank     bank where assembly is stored\n"
    "-feat         write contig features into the bank\n"
    "-recompute    recompute library sizes\n"
    "-update       update bank with recomputed library sizes\n"
    "-minobs <n>   minimum number of good mate-pairs required to recompute\n"
    "              library sizes\n"
    "-numsd <n>    mate-pairs within <n> standard deviations of the mean\n"
    "              library size are considered good. <n> can be fractional\n"
    "-goodcvg <n>  report regions with good mate coverage less than <n>\n"
    "-shortcvg <n> report regions with short mate coverage greater than <n>\n"
    "-longcvg <n>  report regions with long mate coverage greater than <n>\n"
    "-samecvg <n>  report regions with coverage by mates with same orientation\n"
    "              greater than <n>\n"
    "-outiecvg <n> report regions with outie coverage greater than <n>\n"
    "-debug        output status for each mate-pair to STDERR\n"
    "\n.DESCRIPTION.\n"
    "\n.KEYWORDS.\n"
    "AMOS bank, Validation, Mate pairs\n"
       << endl;
}

bool GetOptions(int argc, char ** argv)
{  
  int option_index = 0;
  static struct option long_options[] = {
    {"help",      0, 0, 'h'},
    {"h",         0, 0, 'h'},
    {"b",         1, 0, 'b'},
    {"bank",      1, 0, 'b'},
    {"minobs",    1, 0, 'm'},
    {"numsd",     1, 0, 'S'},
    {"goodcvg",   1, 0, 'g'},
    {"shortcvg",  1, 0, 's'},
    {"longcvg",   1, 0, 'l'},
    {"samecvg",   1, 0, 'n'},
    {"outiecvg",  1, 0, 'o'},
    {"recompute", 0, 0, 'r'},
    {"update",    0, 0, 'u'},
    {"feat",      0, 0, 'f'},
    {"debug",     0, 0, 'd'},
    {0, 0, 0, 0}
  };
  
  int c;
  while ((c = getopt_long_only(argc, argv, "", long_options, &option_index))!= -
         1){
    switch (c){
    case 'h':
      printHelpText();
      exit(0);
      break;
    case 'b':
      globals["bank"] = string(optarg);
      break;
    case 'm':
      globals["minobs"] = string(optarg);
      break;
    case 'S':
      globals["numsd"] = string(optarg);
      break;
    case 'g':
      globals["goodcvg"] = string(optarg);
      break;
    case 's':
      globals["shortcvg"] = string(optarg);
      break;
    case 'l':
      globals["longcvg"] = string(optarg);
      break;
    case 'n':
      globals["normalcvg"] = string(optarg);
      break;
    case 'o':
      globals["outiecvg"] = string(optarg);
      break;
    case 'r':
      globals["recompute"] = string("true");
      break;
    case 'u':
      globals["update"] = string("true");
      break;
    case 'f':
      globals["feat"] = string("true");
      break;
    case 'd':
      globals["debug"] = string("true");
      break;
    case '?':
      return false;
    }
  }

  return true;
} // GetOptions


// returns the distance between the reads implied by the ranges within
// the contig of length Len.  Returns 0 if the reads are improperly
// oriented or if they are too close to the end of the contig (closer than End)
int mateLen (AnnotatedMatePair & m, Range_t & a, Range_t & b, int Len, int End)
{
  int oriA;
  int oriB;

  if (a.getBegin() < a.getEnd()) 
    oriA = 1;
  else 
    oriA = -1;

  if (b.getBegin() < b.getEnd())
    oriB = 1;
  else
    oriB = -1;

  if (oriA == oriB){ // A --> B -->
    m.status = MP_NORMAL;
    return 0;
  }

  if (oriA == 1 && a.getBegin() > b.getBegin()){ // B <--  --> A
    m.status = MP_OUTIE;
    return 0;
  }

  if (oriA == -1 && a.getBegin() < b.getBegin()){ // A <-- --> B
    m.status = MP_OUTIE;
    return 0;
  }

  // now everything is kosher A --> <-- B or B --> <-- A
  if ((oriA == 1 && (Len - a.getBegin() < End))||
      (oriA == -1 && (a.getBegin() < End)) ||
      (oriB == 1 && (Len - b.getBegin() < End)) ||
      (oriB == -1 && (b.getBegin() < End))){
    return 0; // mate-pair too close to end to count
  }

  return abs(a.getBegin() - b.getBegin());
}// mateLen


// computes mean and standard deviation for a set of observations
// observations outside of 5 standard deviations are excluded
pair<Pos_t, SD_t> getSz(list<Pos_t> & sizes)
{
  int numObs = sizes.size();
  if (numObs == 0) return pair<Pos_t, SD_t> (0, 0); 
  // this should probably be an assert...

  Pos_t mean = 0;
  for (list<Pos_t>::iterator li = sizes.begin();  li != sizes.end(); li++){
    mean += *li;
  }
  mean = (Pos_t) round (mean * 1.0 / numObs);

  SD_t stdev = 0;
  for (list<Pos_t>::iterator li = sizes.begin();  li != sizes.end(); li++){
    stdev += (*li - mean) * (*li - mean);
  }
  stdev = (SD_t) round(sqrt(stdev * 1.0 / (numObs - 1)));
  
  Pos_t origm = mean;
  SD_t origs = stdev;

  // recompute mean and stdev for just sizes within 5SD of mean

  mean = 0;
  for (list<Pos_t>::iterator li = sizes.begin();  li != sizes.end(); li++){
    if (abs(*li - origm) < 5 * origs)
      mean += *li;
    else 
      numObs--;
  }

  if (numObs == 0) return pair<Pos_t, SD_t> (0, 0);
  // this should also be an assert

  mean = (Pos_t) round (mean * 1.0 / numObs);
 
  stdev = 0;
  for (list<Pos_t>::iterator li = sizes.begin();  li != sizes.end(); li++){
    if (abs(*li - origm) < 5 * origs)
      stdev += (*li - mean) * (*li - mean);
  }
  stdev = (SD_t) round(sqrt(stdev * 1.0 / (numObs - 1)));

  return pair<Pos_t, SD_t>(mean, stdev);
}

// get regions that are below or above a specified coverage
void getCvg(list<pair<Pos_t, Pos_t> > & ranges,
	    list<pair<Pos_t, Pos_t> > & interest,
	    Pos_t ctglen, int coverage, bool above)
{
  vector<Pos_t> starts, ends;

  interest.clear();
  
  if (ranges.size() == 0 && above) return;
  if (ranges.size() == 0 && coverage >= 0) {
    // all contig is below needed coverage
    interest.push_back(pair<Pos_t, Pos_t>(0, ctglen));
    return;
  }

  for (list<pair<Pos_t, Pos_t> >::iterator li = ranges.begin(); 
       li != ranges.end(); li++){
    //    cout << "Adding " << li->first << ", " << li->second << endl;
    starts.push_back(li->first);
    ends.push_back(li->second);
  }

  sort(starts.begin(), starts.end(), less<Pos_t>());
  sort(ends.begin(), ends.end(), less<Pos_t>());

  vector<Pos_t>::iterator si, ei;
  
  si = starts.begin(); ei = ends.begin();
  int cvg = 0;
  Pos_t s = 0, e = ctglen;
  while (si != starts.end() && ei != ends.end()){
    if (*si <= *ei){
      if (! above && cvg == coverage){
	e = *si;
	interest.push_back(pair<Pos_t, Pos_t>(s, e));
      }
      cvg++;
      if (above && cvg == coverage)
	s = *si;
      si++;
    } else {
      if (above && cvg == coverage) {
	e = *ei;
	interest.push_back(pair<Pos_t, Pos_t>(s, e));
      }
      cvg--;
      if (! above && cvg == coverage)
	s = *ei;
      ei++;
    }
  }
  
  assert(si == starts.end());
  while (ei != ends.end()){
    if (above && cvg == coverage){
      e = *ei;
      interest.push_back(pair<Pos_t, Pos_t>(s, e));
    }
    cvg--;
    if (! above && cvg == coverage)
      s = *ei;
    ei++;
  }
  if (! above && s != ctglen)
    interest.push_back(pair<Pos_t, Pos_t>(s, ctglen));
  
  return;
} // getCvg

//----------------------------------------------
int main(int argc, char **argv)
{
  if (! GetOptions(argc, argv)){
    cerr << "Command line parsing failed" << endl;
    printHelpText();
    exit(1);
  }

  // set up some global variables
  int MIN_OBS = 100;   // min. num. of observations required to change lib size
  float NUM_SD = 3.00; // num. of standard deviations within which mate is good
  int MIN_GOOD_CVG = 0; // good mate coverages below this will be reported
  int MAX_SHORT_CVG = 2; // short mate coverages over this will be reported
  int MAX_LONG_CVG = 2; // long mate coverages over this will be reported
  int MAX_NORMAL_CVG = 3; // normal orientation read coverages > will be reptd.
  int MAX_OUTIE_CVG = 3; // outie orientation read coverages > will be reptd.

  bool recompute = false; // recompute library sizes
  bool update = false;    // update library sizes in bank
  bool feats = false;     // add features to bank
  bool debug = false;     // write debugging info to STDERR

  if (globals.find("debug") != globals.end()){
    debug = true;
    cout << "Debugging mode: additional information written to STDERR" << endl;
  }

  if (globals.find("recompute") != globals.end()){
    recompute = true;
    cout << "Will recompute library sizes" << endl;
  } else
    cout << "Will not recompute library sizes" << endl;

  if (globals.find("update") != globals.end()){
    update = true;
    cout << "Will update libraries in bank" << endl;
  } else
    cout << "Will not update libraries in bank" << endl;

  if (globals.find("feat") != globals.end()){
    feats = true;
    cout << "Will write features in bank" << endl;
  } else 
    cout << "Will not write features in bank" << endl;

  if (globals.find("minobs") != globals.end())
    MIN_OBS = strtol(globals["minobs"].c_str(), NULL, 10);
  cout << "Minimum number of observations required to change library size: " 
       << MIN_OBS << endl;

  if (globals.find("numsd") != globals.end())
    NUM_SD = strtof(globals["numsd"].c_str(), NULL);
  cout << "Mates within " << NUM_SD 
       << " standard deviations from mean considered good" << endl;

  if (globals.find("goodcvg") != globals.end())
    MIN_GOOD_CVG = strtol(globals["goodcvg"].c_str(), NULL, 10);
  cout << "Reporting regions with less than " << MIN_GOOD_CVG
       << " good mate coverage" << endl;

  if (globals.find("shortcvg") != globals.end())
    MAX_SHORT_CVG = strtol(globals["shortcvg"].c_str(), NULL, 10);
  cout << "Reporting regions with more than " << MAX_SHORT_CVG
       << " short mate coverage" << endl;

  if (globals.find("longcvg") != globals.end())
    MAX_LONG_CVG = strtol(globals["longcvg"].c_str(), NULL, 10);
  cout << "Reporting regions with more than " << MAX_LONG_CVG
       << " long mate coverage" << endl;

  if (globals.find("normalcvg") != globals.end())
    MAX_NORMAL_CVG = strtol(globals["normalcvg"].c_str(), NULL, 10);
  cout << "Reporting regions with more than " << MAX_NORMAL_CVG 
       << " coverage by mates oriented the same way" << endl;

  if (globals.find("outiecvg") != globals.end())
    MAX_OUTIE_CVG = strtol(globals["outiecvg"].c_str(), NULL, 10);
  cout << "Reporting regions with more than " << MAX_OUTIE_CVG
       << " coverage by outie mates" << endl;

  cout << endl;

  // open necessary files
  if (globals.find("bank") == globals.end()){ // no bank was specified
    cerr << "A bank must be specified" << endl;
    exit(1);
  }

  // prepare banks we'll need
  
  Bank_t library_bank (Library_t::NCODE);
  if (! library_bank.exists(globals["bank"])){
    cerr << "No library account found in bank " << globals["bank"] << endl;
    exit(1);
  }
  try {
    if (update)
      library_bank.open(globals["bank"], B_READ|B_WRITE);
    else
      library_bank.open(globals["bank"], B_READ);
  } catch (Exception_t & e)
    {
      cerr << "Failed to open library account in bank " << globals["bank"] 
           << ": " << endl << e << endl;
      exit(1);
    }

  Bank_t contig_bank (Contig_t::NCODE);
  BankStream_t contig_stream (Contig_t::NCODE);
  if (! contig_bank.exists(globals["bank"])){
    cerr << "No contig account found in bank " << globals["bank"] << endl;
    exit(1);
  }
  try {
    contig_stream.open(globals["bank"], B_READ);
  } catch (Exception_t & e)
    {
      cerr << "Failed to open contig account in bank " << globals["bank"] 
           << ": " << endl << e << endl;
      exit(1);
    }

  BankStream_t mate_bank (Matepair_t::NCODE);
  if (! mate_bank.exists(globals["bank"])){
    cerr << "No mate account found in bank " << globals["bank"] << endl;
    exit(1);
  }
  try {
    mate_bank.open(globals["bank"], B_READ);
  } catch (Exception_t & e)
    {
      cerr << "Failed to open mate account in bank " << globals["bank"] 
           << ": " << endl << e << endl;
      exit(1);
    }
  
  Bank_t read_bank (Read_t::NCODE);
  if (! read_bank.exists(globals["bank"])){
    cerr << "No read account found in bank " << globals["bank"] << endl;
    exit(1);
  }
  try {
    read_bank.open(globals["bank"], B_READ);
  } catch (Exception_t & e)
    {
      cerr << "Failed to open read account in bank " << globals["bank"] 
           << ": " << endl << e << endl;
      exit(1);
    }


  Bank_t frag_bank (Fragment_t::NCODE);
  if (! frag_bank.exists(globals["bank"])){
    cerr << "No fragment account found in bank " << globals["bank"] << endl;
    exit(1);
  }
  try {
    frag_bank.open(globals["bank"], B_READ);
  } catch (Exception_t & e)
    {
      cerr << "Failed to open fragment account in bank " << globals["bank"] 
           << ": " << endl << e << endl;
      exit(1);
    }


  // Stream through the contigs and retrieve map of read positions within ctgs.
  Contig_t ctg;
  
  hash_map<ID_t, ID_t, hash<ID_t>, equal_to<ID_t> > rd2ctg;     // map from read to contig
  hash_map<ID_t, Range_t, hash<ID_t>, equal_to<ID_t> > rd2posn; // position in contig
  hash_map<ID_t, Size_t, hash<ID_t>, equal_to<ID_t> > ctglen;   // length of contig
  hash_map<ID_t, string, hash<ID_t>, equal_to<ID_t> > ctgname;   // name of contig

  set<ID_t> ctgIDs;
  while (contig_stream >> ctg)
    for (vector<Tile_t>::iterator ti = ctg.getReadTiling().begin(); 
         ti != ctg.getReadTiling().end(); ti++){
      rd2ctg[ti->source] = ctg.getIID();
      ctgIDs.insert(ctg.getIID());
      Pos_t f, l; // coords of beginning/end of read
      if (ti->range.end < ti->range.begin) { // reverse
	f = ti->offset + ti->range.begin - ti->range.end;
	l = ti->offset;
      } else {
	f = ti->offset;
	l = ti->offset + ti->range.end - ti->range.begin;
      }
      rd2posn[ti->source] = 
        Range_t(f, l);

      ctglen[ctg.getIID()] = ctg.getLength();
      ctgname[ctg.getIID()] = ctg.getEID();
    }

  contig_stream.close();

  AnnotatedMatePair mtp;
  list<AnnotatedMatePair> mtl;
  Read_t rd1, rd2;
  Fragment_t frg;
  set<ID_t> libIDs;
  hash_map<ID_t, ID_t, hash<ID_t>, equal_to<ID_t> > rd2lib;
  hash_map<ID_t, ID_t, hash<ID_t>, equal_to<ID_t> > rd2frg;
  hash_map<ID_t, list<list<AnnotatedMatePair>::iterator>, hash<ID_t>, equal_to<ID_t> > lib2mtp;
  hash_map<ID_t, list<list<AnnotatedMatePair>::iterator>, hash<ID_t>, equal_to<ID_t> > ctg2mtp;
  hash_map<ID_t, string, hash<ID_t>, equal_to<ID_t> > rd2name;

  while (mate_bank >> mtp)
    if (rd2ctg[mtp.getReads().first] == rd2ctg[mtp.getReads().second]){
      // mate-pair between reads within the same contig
      list<AnnotatedMatePair>::iterator ti; 

      //      ctgIDs.insert(rd2ctg[mtp.getReads().first]);
      ti = mtl.insert(mtl.end(), mtp);
      read_bank.fetch(mtp.getReads().first, rd1); // get the read
      read_bank.fetch(mtp.getReads().second, rd2);
      rd2name[rd1.getIID()] = rd1.getEID();
      rd2name[rd2.getIID()] = rd2.getEID();

      frag_bank.fetch(rd1.getFragment(), frg); // get the fragment
      rd2frg[mtp.getReads().first] = rd1.getFragment();
      libIDs.insert(frg.getLibrary());
      rd2lib[mtp.getReads().first] = frg.getLibrary();
      lib2mtp[frg.getLibrary()].push_back(ti);
      ctg2mtp[rd2ctg[mtp.getReads().first]].push_back(ti);
    } else {
      if (debug)
	cerr << "Mates " 
	     << mtp.getReads().first 
	     << "(" << rd2name[mtp.getReads().first] << ") " 
	     << mtp.getReads().second 
	     << "(" << rd2name[mtp.getReads().second] << ") "
	     << "not in same contig: "
	     <<	 rd2ctg[mtp.getReads().first] << ", "
	     <<	 rd2ctg[mtp.getReads().second] 
	     << endl;
    }
	
  
  // now we get the library information for each library
  hash_map<ID_t, pair<Pos_t, SD_t>, hash<ID_t>, equal_to<ID_t> > lib2size;
  hash_map<ID_t, string, hash<ID_t>, equal_to<ID_t> > lib2name;
  Library_t lib;
  for (set<ID_t>::iterator li = libIDs.begin(); li != libIDs.end(); li++){
    library_bank.fetch(*li, lib);
    lib2name[*li] = lib.getEID();
    lib2size[*li] = pair<Pos_t, SD_t> (lib.getDistribution().mean, 
				       lib.getDistribution().sd);
  }
  
  // now we're ready to figure out new library sizes
  hash_map<ID_t, pair<Pos_t, SD_t>, hash<ID_t>, equal_to<ID_t> > NewLib2size;
  
  for (set<ID_t>::iterator li = libIDs.begin(); li != libIDs.end(); li++){
    // for each library
    list<Pos_t> sizes;
    pair<Pos_t, SD_t> libsize = lib2size[*li];
    
    for (list<list<AnnotatedMatePair>::iterator>::iterator 
	   mi = lib2mtp[*li].begin(); mi != lib2mtp[*li].end(); mi++){
      // for each mate pair
      Pos_t sz = mateLen(**mi, rd2posn[(*mi)->getReads().first], 
			 rd2posn[(*mi)->getReads().second], 
			 ctglen[rd2ctg[(*mi)->getReads().first]], 
			 libsize.first + 3 * libsize.second);

      if (recompute && sz != 0) {
	sizes.push_back(sz);
      }
    } // if recomputing library sizes

    // compute new sizes
    pair<Pos_t, SD_t> newSize;
    if (! recompute || sizes.size() < MIN_OBS ){
      newSize = libsize;
    } else {
      newSize = getSz(sizes);
      if (newSize != libsize){
	cout <<  "Library " << lib2name[*li] << " recomputed as mean = " 
	     << newSize.first << " SD = " << newSize.second << endl;
	if (update){
	  Library_t newlib;
	  Distribution_t dist;
	  dist.mean = newSize.first;
	  dist.sd = newSize.second;
	  newlib.setIID(*li);
	  newlib.setEID(lib2name[*li]);
	  newlib.setDistribution(dist);
	  try {
	    library_bank.replace(*li, newlib);
	  }  
	  catch (Exception_t & e)
	    {
	      cerr << "Failed to replace library " << *li << "(" 
		   << lib2name[*li] << ") in bank " << globals["bank"] 
		   << ": " << endl << e << endl;
	      exit(1);
	    }
	} // if updating library in bank
      } // if size actually changed
    } 
    NewLib2size[*li] = newSize;

    // recheck mate lengths and set status
    for (list<list<AnnotatedMatePair>::iterator>::iterator 
	   mi = lib2mtp[*li].begin(); mi != lib2mtp[*li].end(); mi++){
      // for each mate pair
      // turn off end check 
      if ((*mi)->status != MP_GOOD)
	continue; // only interested in mates not already tagged

      Pos_t sz = mateLen(**mi, rd2posn[((*mi)->getReads()).first], 
			 rd2posn[((*mi)->getReads()).second], 
			 ctglen[rd2ctg[((*mi)->getReads()).first]], 0);

      if (debug && sz != 0)
	cerr << "MATE " << *li << " " << sz << endl;
      
      // sz == 0 implies mis-orientation
      if (abs(sz - newSize.first) > newSize.second * NUM_SD) {
	// farther than NUM_SD standard deviations from the mean
	if (sz < newSize.first){
	  (*mi)->status = MP_SHORT; // too short
	  (*mi)->deviation = newSize.first - sz;
	  if (debug)
	    cerr << "Short mate " 
		 << ((*mi)->getReads()).first 
		 << "(" << rd2name[((*mi)->getReads()).first] << ") " 
		 << ((*mi)->getReads()).second 
		 << "(" << rd2name[((*mi)->getReads()).second] << ") "
		 << (*mi)->deviation << " shorter than " << newSize.first
		 << endl;
		
	} else { 
	  (*mi)->status = MP_LONG; // too long
	  (*mi)->deviation = sz - newSize.first;
	  if (debug)
	    cerr << "Long mate " 
		 << ((*mi)->getReads()).first 
		 << "(" << rd2name[((*mi)->getReads()).first] << ") " 
		 << ((*mi)->getReads()).second 
		 << "(" << rd2name[((*mi)->getReads()).second] << ") "
		 << (*mi)->deviation << " longer than " << newSize.first
		 << endl;
	}
      }
    }
  }// for each library

  if (recompute) 
    cout << endl;

  if (feats)
    contig_bank.open(globals["bank"], B_READ|B_WRITE);
    
  for (set<ID_t>::iterator ctg = ctgIDs.begin(); ctg != ctgIDs.end(); ctg++) {

    list<pair<Pos_t, Pos_t> > ranges;  // ranges we are interested in
    list<pair<Pos_t, Pos_t> > interest;

    cout << ">Contig_" << *ctg << " " << ctgname[*ctg] << " " 
	 << ctglen[*ctg] << " bases" << endl;

    Contig_t newcontig;
    vector<Feature_t> features;

    if (feats){
      try {
	contig_bank.fetch(*ctg, newcontig);
      }
      catch (Exception_t & e)
	{
	  cerr << "Failed to retrieve contig " << *ctg 
	       << " from bank " << globals["bank"] 
	       << ": " << endl << e << endl;
	  exit(1);
	}
    }

    // find coverage by good mates
    for (list<list<AnnotatedMatePair>::iterator>::iterator 
	   mi = ctg2mtp[*ctg].begin(); mi != ctg2mtp[*ctg].end(); mi++){
      if ((*mi)->status == MP_GOOD){
	Pos_t left, right;
	left = rd2posn[((*mi)->getReads()).first].getEnd(); // use end of reads
	right = rd2posn[((*mi)->getReads()).second].getEnd(); 

	if (left < right)
	  ranges.push_back(pair<Pos_t, Pos_t>(left, right));
	else
	  ranges.push_back(pair<Pos_t, Pos_t>(right, left));

	if (debug)
	  cerr << "Good mate " 
	       << ((*mi)->getReads()).first 
	       << "(" << rd2name[((*mi)->getReads()).first] << ") " 
	       << ((*mi)->getReads()).second 
	       << "(" << rd2name[((*mi)->getReads()).second] << ") "
	       << endl;
      }
    } // for each mate in the contig

    getCvg(ranges, interest, ctglen[*ctg], MIN_GOOD_CVG, false); 

    // report interesting ranges

    if (interest.size() > 0){
      cout << "Regions of " << MIN_GOOD_CVG << "X" 
	   << ((MIN_GOOD_CVG != 0) ? " or less": "") << " clone coverage" 
	   <<endl;
      
      for (list<pair<Pos_t, Pos_t> >::iterator ii = interest.begin(); 
	   ii != interest.end(); ii++){
	cout << ii->first << "\t" << ii->second << endl;
	if (feats){
	  Feature_t f;
	  f.group = "LOW_GOOD_CVG";
	  f.range = Range_t(ii->first, ii->second);
	  f.type = Feature_t::COVERAGE;
	  features.push_back(f);
	}
      }
      cout << endl;
    }

    ranges.clear();

    // find coverage by short mates
    for (list<list<AnnotatedMatePair>::iterator>::iterator 
	   mi = ctg2mtp[*ctg].begin(); mi != ctg2mtp[*ctg].end(); mi++){
      if ((*mi)->status == MP_SHORT){
	Pos_t left, right;
	// use beginnings of reads
	left = rd2posn[((*mi)->getReads()).first].getBegin(); 
	right = rd2posn[((*mi)->getReads()).second].getBegin(); 

	if (left < right)
	  ranges.push_back(pair<Pos_t, Pos_t>(left, right));
	else
	  ranges.push_back(pair<Pos_t, Pos_t>(right, left));
      }
    } // for each mate in the contig

    getCvg(ranges, interest, ctglen[*ctg], MAX_SHORT_CVG, true); 

    // report interesting ranges

    if (interest.size() > 0){
      cout << "Regions of " << MAX_SHORT_CVG << "X or more short mate coverage"
	   <<endl;
      
      for (list<pair<Pos_t, Pos_t> >::iterator ii = interest.begin(); 
	   ii != interest.end(); ii++){
	cout << ii->first << "\t" << ii->second << endl;
	if (feats){
	  Feature_t f;
	  f.group = "HIGH_SHORT_CVG";
	  f.range = Range_t(ii->first, ii->second);
	  f.type = Feature_t::COVERAGE;
	  features.push_back(f);
	}
      }
      cout << endl;
    }

    ranges.clear();

    // find coverage by long mates
    for (list<list<AnnotatedMatePair>::iterator>::iterator 
	   mi = ctg2mtp[*ctg].begin(); mi != ctg2mtp[*ctg].end(); mi++){
      if ((*mi)->status == MP_LONG){
	Pos_t left, right;
	// use beginnings of reads
	left = rd2posn[((*mi)->getReads()).first].getBegin(); 
	right = rd2posn[((*mi)->getReads()).second].getBegin(); 

	if (left < right)
	  ranges.push_back(pair<Pos_t, Pos_t>(left, right));
	else
	  ranges.push_back(pair<Pos_t, Pos_t>(right, left));
      }
    } // for each mate in the contig

    getCvg(ranges, interest, ctglen[*ctg], MAX_LONG_CVG, true); 

    // report interesting ranges

    if (interest.size() > 0){
      cout << "Regions of " << MAX_LONG_CVG << "X or more long mate coverage" 
	   <<endl;
      
      for (list<pair<Pos_t, Pos_t> >::iterator ii = interest.begin(); 
	   ii != interest.end(); ii++){
	cout << ii->first << "\t" << ii->second << endl;
	if (feats){
	  Feature_t f;
	  f.group = "HIGH_LONG_CVG";
	  f.range = Range_t(ii->first, ii->second);
	  f.type = Feature_t::COVERAGE;
	  features.push_back(f);
	}
      }
      cout << endl;
    }
    
    ranges.clear();

    // find coverage by normal reads
    for (list<list<AnnotatedMatePair>::iterator>::iterator 
	   mi = ctg2mtp[*ctg].begin(); mi != ctg2mtp[*ctg].end(); mi++){
      if ((*mi)->status == MP_NORMAL){
	Pos_t left, right;

	left = rd2posn[((*mi)->getReads()).first].getBegin(); 
	right = rd2posn[((*mi)->getReads()).first].getEnd(); 
	if (left < right)
	  ranges.push_back(pair<Pos_t, Pos_t>(left, right));
	else
	  ranges.push_back(pair<Pos_t, Pos_t>(right, left));

	left = rd2posn[((*mi)->getReads()).second].getBegin(); 
	right = rd2posn[((*mi)->getReads()).second].getEnd(); 
	if (left < right)
	  ranges.push_back(pair<Pos_t, Pos_t>(left, right));
	else
	  ranges.push_back(pair<Pos_t, Pos_t>(right, left));
	if (debug)
	  cerr << "Same orientation mates " 
	       << ((*mi)->getReads()).first 
	       << "(" << rd2name[((*mi)->getReads()).first] << ") " 
	       << ((*mi)->getReads()).second 
	       << "(" << rd2name[((*mi)->getReads()).second] << ") "
	       << endl;
      }
    } // for each mate in the contig

    getCvg(ranges, interest, ctglen[*ctg], MAX_NORMAL_CVG, true); 

    // report interesting ranges

    if (interest.size() > 0){
      cout << "Regions of " << MAX_NORMAL_CVG 
	   << "X or more same-orientation coverage" <<endl;
      
      for (list<pair<Pos_t, Pos_t> >::iterator ii = interest.begin(); 
	   ii != interest.end(); ii++){
	cout << ii->first << "\t" << ii->second << endl;
	if (feats){
	  Feature_t f;
	  f.group = "HIGH_NORMAL_CVG";
	  f.range = Range_t(ii->first, ii->second);
	  f.type = Feature_t::COVERAGE;
	  features.push_back(f);
	}
      }
      cout << endl;
    }

    ranges.clear();

    // find coverage by outie reads
    for (list<list<AnnotatedMatePair>::iterator>::iterator 
	   mi = ctg2mtp[*ctg].begin(); mi != ctg2mtp[*ctg].end(); mi++){
      if ((*mi)->status == MP_OUTIE){
	Pos_t left, right;

	if ((*mi)->getType() == Matepair_t::TRANSPOSON)
	  continue;  // transposons are not errors
	left = rd2posn[((*mi)->getReads()).first].getBegin(); 
	right = rd2posn[((*mi)->getReads()).first].getEnd(); 
	if (left < right)
	  ranges.push_back(pair<Pos_t, Pos_t>(left, right));
	else
	  ranges.push_back(pair<Pos_t, Pos_t>(right, left));

	left = rd2posn[((*mi)->getReads()).second].getBegin(); 
	right = rd2posn[((*mi)->getReads()).second].getEnd(); 
	if (left < right)
	  ranges.push_back(pair<Pos_t, Pos_t>(left, right));
	else
	  ranges.push_back(pair<Pos_t, Pos_t>(right, left));

	if (debug)
	  cerr << "Outie mates " 
	       << ((*mi)->getReads()).first 
	       << "(" << rd2name[((*mi)->getReads()).first] << ") " 
	       << ((*mi)->getReads()).second 
	       << "(" << rd2name[((*mi)->getReads()).second] << ") "
	       << endl;
      }
    } // for each mate in the contig

    getCvg(ranges, interest, ctglen[*ctg], MAX_OUTIE_CVG, true); 

    // report interesting ranges

    if (interest.size() > 0){
      cout << "Regions of " << MAX_OUTIE_CVG << "X or more outie coverage" 
	   <<endl;
      
      for (list<pair<Pos_t, Pos_t> >::iterator ii = interest.begin(); 
	   ii != interest.end(); ii++){
	cout << ii->first << "\t" << ii->second << endl;
	if (feats){
	  Feature_t f;
	  f.group = "HIGH_OUTIE_CVG";
	  f.range = Range_t(ii->first, ii->second);
	  f.type = Feature_t::COVERAGE;
	  features.push_back(f);
	}
      }
      cout << endl;
    }

    if (feats){
      newcontig.setFeatures(features);
      contig_bank.replace(newcontig.getIID(), newcontig);
    }

  } // for each contig

  frag_bank.close();
  read_bank.close();
  library_bank.close();
  if (feats){
    contig_bank.clean(); // features may have messed up the bank
    contig_bank.close();
  }
  mate_bank.close();

  return(0);
} // main
    
