////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Adam M Phillippy
//! \date 03/08/2004
//!
//! \brief Dumps a bambus .mates file from an AMOS bank
//!
////////////////////////////////////////////////////////////////////////////////

#include "universals_AMOS.hh"
#include <iostream>
#include <cassert>
#include <unistd.h>
using namespace std;
using namespace AMOS;


//=============================================================== Globals ====//
string OPT_BankName;                 // bank name parameter


//========================================================== Fuction Decs ====//
//----------------------------------------------------- ParseArgs --------------
//! \brief Sets the global OPT_% values from the command line arguments
//!
//! \return void
//!
void ParseArgs (int argc, char ** argv);


//----------------------------------------------------- PrintHelp --------------
//! \brief Prints help information to cerr
//!
//! \param s The program name, i.e. argv[0]
//! \return void
//!
void PrintHelp (const char * s);


//----------------------------------------------------- PringUsage -------------
//! \brief Prints usage information to cerr
//!
//! \param s The program name, i.e. argv[0]
//! \return void
//!
void PrintUsage (const char * s);


//========================================================= Function Defs ====//
int main (int argc, char ** argv)
{
  Read_t red;
  Fragment_t frg;
  Library_t lib;
  Matepair_t mtp;

  Bank_t red_bank (Bank_k::READ);
  Bank_t frg_bank (Bank_k::FRAGMENT);
  Bank_t lib_bank (Bank_k::LIBRARY);
  Bank_t mtp_bank (Bank_k::MATEPAIR);

  ID_t id;                       // id holder
  long int cnts = 0;             // seen object count
  long int cntw = 0;             // written object count

  //-- Parse the command line arguments
  ParseArgs (argc, argv);

  //-- BEGIN: MAIN EXCEPTION CATCH
  try {

    mtp_bank . open (OPT_BankName);
    red_bank . open (OPT_BankName);
    frg_bank . open (OPT_BankName);
    lib_bank . open (OPT_BankName);

    //-- Iterate through each library in the bank
    for ( id = 1; id <= lib_bank . getLastIID( ); id ++ )
      {
	//-- Fetch the next object
	lib . setIID (id);
	lib_bank . fetch (lib);
	if ( lib . isRemoved( ) )
	  continue;

	cout << "library\t" << lib . getIID( ) << '\t'
	     << lib . getDistribution( ) . mean -
	  (lib . getDistribution( ) . sd * 3) << '\t'
	     << lib . getDistribution( ) . mean +
	  (lib . getDistribution( ) . sd * 3) << endl;
      }

    //-- Iterate through each object in the bank
    for ( id = 1; id <= mtp_bank . getLastIID( ); id ++ )
      {
	//-- Fetch the next object
	mtp . setIID (id);
	mtp_bank . fetch (mtp);
	if ( mtp . isRemoved( ) )
	  continue;
	cnts ++;

	//-- Get the library record for this insert
	red . setIID (mtp . getReads( ) . first);
	red_bank . fetch (red);
	frg . setIID (red . getFragment( ));
	frg_bank . fetch (frg);

	cout << mtp . getReads( ) . first << '\t'
	     << mtp . getReads( ) . second << '\t'
	     << frg . getLibrary( ) << endl;

	cntw ++;
      }

    mtp_bank . close( );
    red_bank . close( );
    frg_bank . close( );
    lib_bank . close( );
  }
  catch (Exception_t & e) {

  //-- On error, print debugging information
  cerr << "Objects seen: " << cnts << endl
       << "Objects written: " << cntw << endl
       << "ERROR: -- Fatal AMOS Exception --\n" << e;
  return EXIT_FAILURE;
  }
  //-- END: MAIN EXCEPTION CATCH

  return EXIT_SUCCESS;
}




//------------------------------------------------------------- ParseArgs ----//
void ParseArgs (int argc, char ** argv)
{
  int ch, errflg = 0;
  optarg = NULL;

  while ( !errflg && ((ch = getopt (argc, argv, "h")) != EOF) )
    switch (ch)
      {
      case 'h':
        PrintHelp (argv[0]);
        exit (EXIT_SUCCESS);
        break;
      default:
        errflg ++;
      }

  if ( errflg > 0 || optind != argc - 1 )
    {
      PrintUsage (argv[0]);
      cerr << "Try '" << argv[0] << " -h' for more information.\n";
      exit (EXIT_FAILURE);
    }

  OPT_BankName = argv [optind ++];
}




//------------------------------------------------------------- PrintHelp ----//
void PrintHelp (const char * s)
{
  PrintUsage (s);
  cerr
    << "-h            Display help information\n\n";

  cerr
    << "Takes an AMOS bank directory and dumps a bambus .mates files to\n"
    << "stdout\n\n";
  return;
}




//------------------------------------------------------------ PrintUsage ----//
void PrintUsage (const char * s)
{
  cerr
    << "\nUSAGE: " << s << "  [options]  <bank path>\n\n";
  return;
}
