////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Adam M Phillippy
//! \date 11/12/2003
//!
//! \brief Header for Message_t and IMessagable_t
//! \todo Validity checking for Message and Field codes
//!
////////////////////////////////////////////////////////////////////////////////

#ifndef __Message_AMOS_HH
#define __Message_AMOS_HH 1

#include "exceptions_AMOS.hh"
#include "inttypes_AMOS.hh"
#include <map>
#include <cctype>
#include <string>
#include <vector>
#include <iostream>




namespace AMOS {

namespace Message_k {

  //-- Enumeration characters
  const char          E_ADD         = 'A';
  const char          E_DELETE      = 'D';
  const char          E_EDIT        = 'E';

  //-- Field NCode types
  const NCode_t   F_NULL         = Encode ("nul");
  const NCode_t   F_3PRIME       = Encode ("3pr");
  const NCode_t   F_5PRIME       = Encode ("5pr");
  const NCode_t   F_AHANG        = Encode ("ahg");
  const NCode_t   F_ACTION       = Encode ("act");
  const NCode_t   F_ADJACENCY    = Encode ("adj");
  const NCode_t   F_BHANG        = Encode ("bhg");
  const NCode_t   F_CLEAR        = Encode ("clr");
  const NCode_t   F_COMMENT      = Encode ("com");
  const NCode_t   F_CONTIG1      = Encode ("ct1");
  const NCode_t   F_CONTIG2      = Encode ("ct2");
  const NCode_t   F_CONTIGEDGE   = Encode ("cte");
  const NCode_t   F_CONTIGLINK   = Encode ("ctl");
  const NCode_t   F_COUNT        = Encode ("cnt");
  const NCode_t   F_DELTA        = Encode ("del");
  const NCode_t   F_EID          = Encode ("eid");
  const NCode_t   F_FRAGMENT     = Encode ("frg");
  const NCode_t   F_LIBRARY      = Encode ("lib");
  const NCode_t   F_MEAN         = Encode ("mea");
  const NCode_t   F_OFFSET       = Encode ("off");
  const NCode_t   F_POLYMORPHISM = Encode ("ply");
  const NCode_t   F_QUALITY      = Encode ("qlt");
  const NCode_t   F_QUALITYCLEAR = Encode ("qcr");
  const NCode_t   F_READ1        = Encode ("rd1");
  const NCode_t   F_READ2        = Encode ("rd2");
  const NCode_t   F_READS        = Encode ("rds");
  const NCode_t   F_SEQUENCE     = Encode ("seq");
  const NCode_t   F_SD           = Encode ("std");
  const NCode_t   F_SIZE         = Encode ("sze");
  const NCode_t   F_SKEWNESS     = Encode ("skw");
  const NCode_t   F_SOURCE       = Encode ("src");
  const NCode_t   F_TYPE         = Encode ("typ");
  const NCode_t   F_VECTORCLEAR  = Encode ("vcr");

  //-- Message NCode types
  const NCode_t   M_NULL         = Encode ("NUL");
  const NCode_t   M_UNIVERSAL    = Encode ("UNV");
  const NCode_t   M_CONTIG       = Encode ("CTG");
  const NCode_t   M_CONTIGEDGE   = Encode ("CTE");
  const NCode_t   M_CONTIGLINK   = Encode ("CTL");
  const NCode_t   M_DISTRIBUTION = Encode ("DST");
  const NCode_t   M_EXAMPLE      = Encode ("EXX");
  const NCode_t   M_FRAGMENT     = Encode ("FRG");
  const NCode_t   M_KMER         = Encode ("KMR");
  const NCode_t   M_LIBRARY      = Encode ("LIB");
  const NCode_t   M_OVERLAP      = Encode ("OVL");
  const NCode_t   M_READ         = Encode ("RED");
  const NCode_t   M_SCAFFOLD     = Encode ("SCF");
  const NCode_t   M_SEQUENCE     = Encode ("SEQ");
  const NCode_t   M_TILE         = Encode ("TLE");

} // namespace Message_k




//================================================ Message_t ===================
//! \brief A generic AMOS message object for reading and writing data
//!
//! Based on the Celera 3-code format, the AMOS 'NCode' messages are designed
//! for easy parsing and compatibility with the other AMOS modules. This
//! generic message class makes no assumptions about the type of data to be
//! stored within, so it can be used for reading and writing ANY type of ASCII
//! record in NCode format. The main duty of this class is to understand and
//! validate incoming or outgoing NCode format.
//!
//==============================================================================
class Message_t
{

private:

  NCode_t mcode_m;                            //!< message NCode type
  std::map<NCode_t,std::string> fields_m;     //!< message fields
  std::vector<Message_t> subs_m;              //!< vector of submessages


public:

  //--------------------------------------------------- Message_t --------------
  //! \brief Constructs an empty Message_t
  //!
  Message_t ( )
  {
    mcode_m = Message_k::M_NULL;
  }


  //--------------------------------------------------- ~Message_t -------------
  //! \brief Destroys a Message_t object
  //!
  ~Message_t ( )
  {

  }


  //--------------------------------------------------- clear ------------------
  //! \brief Clears the message
  //!
  //! \return void
  //!
  void clear ( )
  {
    mcode_m = Message_k::M_NULL;
    fields_m . clear( );
    subs_m . clear( );
  }


  //--------------------------------------------------- exists -----------------
  //! \brief Checks for the existence of a field by NCode
  //!
  //! \param fcode Check for a field with this NCode
  //! \return true if exists, false if otherwise
  //!
  bool exists (NCode_t fcode) const
  {
    return fields_m . find (fcode) == fields_m . end( ) ? false : true;
  }


  //--------------------------------------------------- exists -----------------
  //! \brief Checks for the existence of a field by name
  //!
  //! \param fname Check for a field with this name
  //! \return true if exists, false if otherwise
  //!
  bool exists (const std::string & fname) const
  {
    return exists (Encode (fname));
  }


  //--------------------------------------------------- getField ---------------
  //! \brief Gets field data by field NCode
  //!
  //! Will return the field data, or throws an exception if the field does not
  //! exist. You can check if the field exists first with the exists method.
  //!
  //! \param fcode Get field data with this field NCode
  //! \pre The field code exists in the current message
  //! \throws ArgumentException_t
  //! \return The field data for fcode
  //!
  const std::string & getField (NCode_t fcode) const
  {
    std::map<NCode_t,std::string>::const_iterator mi;

    mi = fields_m . find (fcode);
    if ( mi == fields_m . end( ) )
      AMOS_THROW_ARGUMENT ("Field name does not exist: " + Decode (fcode));

    return mi -> second;
  }


  //--------------------------------------------------- getField ---------------
  //! \brief Gets field data by field name
  //!
  //! Will return the field data, or throws an exception if the field does not
  //! exist. You can check if the field exists first with the exists method.
  //!
  //! \param fname Get field data with this field name
  //! \pre The field name exists in the current message
  //! \throws ArgumentException_t
  //! \return The field data for fname
  //!
  const std::string & getField (const std::string & fname) const
  {
    return getField (Encode (fname));
  }


  //--------------------------------------------------- getSubMessages ---------
  //! \brief Get a the vector of submessages for this message
  //!
  //! \return The vector of submessages for this message
  //!
  const std::vector<Message_t> & getSubMessages ( ) const
  {
    return subs_m;
  }


  //--------------------------------------------------- getMessageCode ---------
  //! \brief Get the NCode type  of this message
  //!
  //! Will return M_NULL if the message has not been assigned a type name
  //! yet. NCode can be decoded to a string with the 'AMOS::Decode' function.
  //!
  //! \return The NCode type of this message
  //!
  NCode_t getMessageCode ( ) const
  {
    return mcode_m;
  }


  //--------------------------------------------------- read -------------------
  //! \brief Read a message from an input stream
  //!
  //! Reads a message from an input stream and populates the message fields
  //! with the values read from the stream. Will throw an exception if a
  //! message is found, but is not properly formatted.
  //!
  //! \param in The input stream to read from
  //! \pre The incoming message is properly formatted
  //! \throws IOException_t
  //! \return true if a message was read, false if no message read (EOF)
  //!
  bool read (std::istream & in);


  //--------------------------------------------------- removeField ------------
  //! \brief Removes a field from the message object by NCode
  //!
  //! Removes a field from the message by its NCode. Has no effect if the field
  //! code does not exist.
  //!
  //! \param fcode The field NCode to remove
  //! \return void
  //!
  void removeField (NCode_t fcode)
  {
    fields_m . erase (fcode);
  }


  //--------------------------------------------------- removeField ------------
  //! \brief Removes a field from the message object
  //!
  //! Removes a field from the message. Has no effect if the field name does
  //! not exist or is improperly formatted.
  //!
  //! \param fname The field name to remove
  //! \return void
  //!
  void removeField (const std::string & fname)
  {
    removeField (Encode (fname));
  }


  //--------------------------------------------------- setSubMessages ---------
  //! \brief Set the vector of submessages for this message
  //!
  //! \param subs The new vector of submessages
  //! \return void
  //!
  void setSubMessages (const std::vector<Message_t> & subs)
  {
    subs_m = subs;
  }


  //--------------------------------------------------- setField ---------------
  //! \brief Set field data by field NCode
  //!
  //! Will set a new, or overwrite and existing field with the supplied data.
  //! Throws an exception for an improperly formatted  data record (i.e. a
  //! multi-line record that does not end in a newline).
  //!
  //! \param fcode The field NCode
  //! \param data The new field data
  //! \pre The data field ends in '\n' if it is multiple lines
  //! \throws ArugmentException_t
  //! \return void
  //!
  void setField (NCode_t fcode, const std::string & data);


  //--------------------------------------------------- setField ---------------
  //! \brief Set field data by field name
  //!
  //! Will set a new, or overwrite and existing field with the supplied data.
  //! Throws an exception for an improperly formatted 3-code field name or
  //! an improperly formated data record (i.e. a multi-line record that does
  //! not end in a newline).
  //!
  //! \param fname The field name
  //! \param data The new field data
  //! \pre The field name is 3 chars
  //! \pre The data field ends in '\n' if it is multiple lines
  //! \throws ArugmentException_t
  //! \return void
  //!
  void setField (const std::string & fname, const std::string & data)
  {
    setField (Encode (fname), data);
  }


  //--------------------------------------------------- setMessageCode ---------
  //! \brief Set the NCode type code of this message
  //!
  //! \param mcode The new NCode type of this message
  //! \return void
  //!
  void setMessageCode (NCode_t mcode)
  {
    mcode_m = mcode;
  }


  //--------------------------------------------------- setMessageCode ---------
  //! \brief Set the NCode type name of this message
  //!
  //! Will throw an exception if the name is improperly formatted.
  //!
  //! \param mname The new NCode type name of this message
  //! \pre The type name is 3 chars
  //! \throws ArgumentException_t
  //! \return void
  //!
  void setMessageCode (const std::string & mname)
  {
    setMessageCode (Encode (mname));
  }


  //--------------------------------------------------- write ------------------
  //! \brief Write the message object to an output stream
  //!
  //! Writes the NCode ASCII representation of this object to the supplied
  //! output stream. Will throw an exception if there was an error trying to
  //! write to the stream.
  //!
  //! \param out The output stream to write to
  //! \pre The ostream is open and writable
  //! \throws IOException_t
  //! \return void
  //!
  void write (std::ostream & out) const;

};





//================================================ IMessagable_t ===============
//! \brief Interface for classes that can interpret messages
//!
//! Provides an interface for classes that can input and output messages. This
//! allows in-memory classes to read and write messages to a stream via the
//! Message_t class.
//!
//==============================================================================
class IMessagable_t
{

public:

  //--------------------------------------------------- IMessagable_t ----------
  //! \brief Constructs a Messagable object with null message type
  //!
  IMessagable_t ( )
  {

  }


  //--------------------------------------------------- ~IMessagable_t ---------
  //! \brief Virtual destructor for Messagable objects
  //!
  virtual ~IMessagable_t ( )
  {

  }


  //--------------------------------------------------- getNCode ---------------
  //! \brief Get the AMOS NCode type identifier
  //!
  //! \return The AMOS NCode type identifier
  //!
  virtual NCode_t getNCode ( ) const = 0;


  //--------------------------------------------------- readMessage ------------
  //! \brief Reads in data from a Message object
  //!
  //! Reads the data contained in a Message object and stores it in the
  //! Messagable object. Will not complain if incoming message is of the wrong
  //! type, will only try and suck out the fields it recognizes. All previous
  //! data in the Messagable object will be cleared or overwritten.
  //!
  //! \param msg The Message to read from
  //! \return void
  //!
  virtual void readMessage (const Message_t & msg) = 0;


  //--------------------------------------------------- writeMessage -----------
  //! \brief Writes data to a Message object
  //!
  //! Writes the data contained in a Messagable object to a Message object.
  //! All previous data in the Message will be cleared or overwritten.
  //!
  //! \param msg The Message to write to
  //! \return void
  //!
  virtual void writeMessage (Message_t & msg) const = 0;

};

} // namespace AMOS

#endif // #ifndef __Message_AMOS_HH
