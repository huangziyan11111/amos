////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Adam M Phillippy
//! \date 06/18/2003
//!
//! \brief Header for Fragment_t
//!
////////////////////////////////////////////////////////////////////////////////

#ifndef __Fragment_AMOS_HH
#define __Fragment_AMOS_HH 1

#include "Universal_AMOS.hh"
#include <vector>




namespace AMOS {

typedef char FragmentType_t;

//================================================ Fragment_t ==================
//! \brief Single piece of DNA
//!
//! A single piece of DNA from which a group of reads were sequenced. This
//! allows for the hierarchical clustering of reads that come from the same
//! fragment.
//!
//==============================================================================
class Fragment_t : public Universal_t
{

public:

  static const FragmentType_t NULL_FRAGMENT  = 0;
  static const FragmentType_t OTHER          = 'X';
  static const FragmentType_t INSERT         = 'I';
  static const FragmentType_t TRANSPOSON     = 'T';
  static const FragmentType_t BAC            = 'B';


private:

  std::pair<ID_t, ID_t> ends_m;    //!< The forward and reverse fragment ends
  ID_t library_m;                  //!< ID of the parent library
  std::vector<ID_t> reads_m;       //!< the list of associated read IDs
  FragmentType_t type_m;           //!< type of fragment


protected:

  //--------------------------------------------------- readRecord -------------
  //! \brief Read all the class members from a biserial record
  //!
  //! Reads the fixed and variable length streams from a biserial record and
  //! initializes all the class members to the values stored within. Used in
  //! translating a biserial Bankable object, and needed to retrieve objects
  //! from a Bank. Returned size of the record will only be valid if the read
  //! was successful, i.e. fix.good( ) and var.good( ).
  //!
  //! \note This method must be able to interpret the biserial record
  //! produced by its related function writeRecord.
  //!
  //! \param fix The fixed length stream (stores all fixed length members)
  //! \param var The variable length stream (stores all var length members)
  //! \pre The get pointer of fix is at the beginning of the record
  //! \pre The get pointer of var is at the beginning of the record
  //! \return size of read record (size of fix + size of var)
  //!
  virtual Size_t readRecord (std::istream & fix,
			     std::istream & var);


  //--------------------------------------------------- writeRecord ------------
  //! \brief Write all the class members to a biserial record
  //!
  //! Writes the fixed an variable length streams to a biserial record. Used in
  //! generating a biserial Bankable object, and needed to commit objects to a
  //! Bank. Will only write to the ready streams, but the size of the record
  //! will always be returned.
  //!
  //! \note This method must be able to produce a biserial record that can
  //! be read by its related funtion readRecord.
  //!
  //! \param fix The fixed length stream (stores all fixed length members)
  //! \param var The variable length stream (stores all var length members)
  //! \return size of written record (size of fix + size of var)
  //!
  virtual Size_t writeRecord (std::ostream & fix,
			      std::ostream & var) const;


public:

  //--------------------------------------------------- BankCode ---------------
  //! \brief Get the AMOS NCode type identifier (statically)
  //!
  //! Used for constructing a Bank with a certain NCode. e.g. 'Bank_t
  //! (Fragment_t::BankCode( ))'
  //!
  //! \return The AMOS NCode type identifier
  //!
  static NCode_t BankCode ( )
  {
    return Bank_k::FRAGMENT;
  }


  //--------------------------------------------------- Fragment_t -------------
  //! \brief Contstructs an empty Fragment_t object
  //!
  //! Sets all members to NULL
  //!
  Fragment_t ( )
  {
    ends_m . first = ends_m . second = library_m = NULL_ID;
    type_m = NULL_FRAGMENT;
  }


  //--------------------------------------------------- Fragment_t -------------
  //! \brief Copy constructor
  //!
  Fragment_t (const Fragment_t & source)
  {
    *this = source;
  }


  //--------------------------------------------------- ~Fragment_t ------------
  //! \brief Destroys a Fragment_t object
  //!
  ~Fragment_t ( )
  {

  }


  //--------------------------------------------------- clear ------------------
  //! \brief Clears all object data, reinitializes the object
  //!
  void clear ( )
  {
    Universal_t::clear( );
    ends_m . first = ends_m . second = library_m = NULL_ID;
    reads_m . clear( );
    type_m = NULL_FRAGMENT;
  }


  //--------------------------------------------------- getNCode ---------------
  //! \brief Get the AMOS NCode type identifier
  //!
  //! \return The AMOS NCode type identifier
  //!
  virtual NCode_t getNCode ( ) const
  {
    return Fragment_t::BankCode( );
  }


  //--------------------------------------------------- getEnds ----------------
  //! \brief Returns the IDs of the 5' and 3' ends of the Fragment
  //!
  //! Returns the IDs of the reads that represent the 5' and 3' ends of the
  //! fragment. Useful for retrieving mate-pair information.
  //!
  //! \return The 5' and 3' ends respectively
  //!
  std::pair<ID_t, ID_t> getEnds ( ) const
  {
    return ends_m;
  }


  //--------------------------------------------------- getLibrary -------------
  //! \brief Get the parent library ID
  //!
  //! \return The ID of the parent library
  //!
  ID_t getLibrary ( ) const
  {
    return library_m;
  }


  //--------------------------------------------------- getReads ---------------
  //! \brief Get the child read IDs
  //!
  //! \return The vector of child read IDs
  //!
  const std::vector<ID_t> & getReads ( ) const
  {
    return reads_m;
  }


  //--------------------------------------------------- getType ----------------
  //! \brief Get the fragment type
  //!
  //! \return The type of fragment
  //!
  FragmentType_t getType ( ) const
  {
    return type_m;
  }


  //--------------------------------------------------- setEnds ----------------
  //! \brief Sets the IDs of the 5' and 3' ends of the Fragment
  //!
  //! Set the IDs of the reads that represent the 5' and 3' ends of the
  //! Fragment. Useful in storing mate-pair information.
  //!
  //! \param ends The 5' and 3' ends respectively
  //! \return void
  //!
  void setEnds (std::pair<ID_t, ID_t> ends)
  {
    ends_m = ends;
  }


  //--------------------------------------------------- setLibrary -------------
  //! \brief Set the parent library ID
  //!
  //! \param library The new ID of parent library
  //! \return void
  //!
  void setLibrary (ID_t library)
  {
    library_m = library;
  }


  //--------------------------------------------------- setReads ---------------
  //! \brief Set the child read IDs
  //!
  //! \param reads The new vector of child read IDs
  //! \return void
  //!
  void setReads (const std::vector<ID_t> & reads)
  {
    reads_m = reads;
  }


  //--------------------------------------------------- setType ----------------
  //! \brief Set the fragment type
  //!
  //! \param type The new fragment type
  //! \return void
  //!
  void setType (FragmentType_t type)
  {
    type_m = type;
  }

};

} // namespace AMOS

#endif // #ifndef __Fragment_AMOS_HH
