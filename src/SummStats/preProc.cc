#include <Sequence/PolyTable.hpp>
#include <Sequence/Ptable.hpp>
#include <Sequence/stateCounter.hpp>
#include <memory>
#include <utility>

/*
  Idea:
  3 classes:
  1. statePreproc
  2. derstatePreproc
  3. HaploPreproc

  The main class contains one of each of them.
  Probably mutable.

  Simplifies iteration down to beg/end, etc.

  template base class to enforce some things?

  Maybe too complex?
 */
namespace Sequence {

  /*
  struct Uhapvec
  {
  };
  */
  
  struct preProcImpl
  {
    //types
    Ptable pt;
    std::vector<std::string> ustrings;
    std::vector< std::vector<std::string>::iterator > haplotypes;

    //constructors
    preProcImpl();
    preProcImpl( const Ptable & );
    preProcImpl( Ptable && );
    preProcImpl( const PolyTable & );
  };

  preProcImpl::preProcImpl() : pt( Ptable() )
  {
  }
  
  preProcImpl::preProcImpl( const Ptable & __pt ) : pt(__pt)
  {
  }

  preProcImpl::preProcImpl( Ptable && __pt) : pt(std::move(__pt))
  {
  }

  preProcImpl::preProcImpl( const PolyTable & __pt) : pt(__pt)
  {
  }

  class preProc //: public std::pair< Ptable, Uhapvec >
  {
  private:
    std::unique_ptr<preProcImpl> __impl;
  public:
    using hap_iterator = std::vector<std::vector<std::string>::iterator >::iterator;
    using hap_const_iterator = std::vector<std::vector<std::string>::iterator >::const_iterator;
    using hap_reference = std::vector<std::vector<std::string>::iterator >::reference;
    using hap_const_reference = std::vector<std::vector<std::string>::iterator >::const_reference;
    using site_iterator = Ptable::iterator;
    using site_const_iterator = Ptable::const_iterator;
    preProc();
    preProc( const Ptable & );
    preProc( Ptable && );
    preProc( const PolyTable & );

    //member functions
    site_iterator begin();
    site_iterator end();
    hap_iterator hap_begin();
    hap_iterator hap_end();
    hap_const_iterator hap_begin() const;
    hap_const_iterator hap_end() const;
    hap_const_iterator hap_cbegin() const;
    hap_const_iterator hap_cend() const;
  };

  preProc::preProc() : __impl(new preProcImpl())
  {
  }
  
  preProc::preProc( const Ptable & __pt) : __impl(new preProcImpl(__pt))
  {
  }
  
  preProc::preProc( Ptable && __pt) : __impl(new preProcImpl(std::move(__pt)))
  {
  }
  
  preProc::preProc( const PolyTable & __pt ) : __impl(new preProcImpl(__pt))
  {
  }
  
  preProc::hap_iterator preProc::hap_begin() {
    return __impl->haplotypes.begin();
  }
  preProc::hap_iterator preProc::hap_end() {
    return __impl->haplotypes.end();
  }
  preProc::hap_const_iterator preProc::hap_begin() const {
    return __impl->haplotypes.cbegin();
  }
  preProc::hap_const_iterator preProc::hap_end() const {
    return __impl->haplotypes.cend();
  }
  preProc::hap_const_iterator preProc::hap_cbegin() const {
    return __impl->haplotypes.cbegin();
  }
  preProc::hap_const_iterator preProc::hap_cend() const {
    return __impl->haplotypes.cend();
  }
}