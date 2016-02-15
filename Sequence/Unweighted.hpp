/*

Copyright (C) 2003-2009 Kevin Thornton, krthornt[]@[]uci.edu

Remove the brackets to email me.

This file is part of libsequence.

libsequence is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

libsequence is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
long with libsequence.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef __UNWEIGHTED_H__
#define __UNWEIGHTED_H__

#include <Sequence/WeightingSchemes.hpp>
/*! \file Unweighted.hpp"
  @brief declares Sequence::Unweighted2 and  Sequence::Unweighted3
*/

/*!
  \class Sequence::Unweighted2 Sequence/Unweighted.hpp
  \ingroup weights
  @short weights all pathways equally
  \note This is generally not what you want to use (it biases the result to a higher Ka/Ks ratio)
*/

/*!
  \class Sequence::Unweighted3 Sequence/Unweighted.hpp
  \ingroup weights
  @short weights all pathways equally
  \note This is generally not what you want to use (it biases the result to a higher Ka/Ks ratio)
*/
namespace Sequence
  {
  struct Unweighted2 : public WeightingScheme2
    {
      weights2_t operator()(const std::string &codon1, const std::string &codon2,Sequence::GeneticCodes genetic_code) const;
    };

    struct Unweighted3 : public WeightingScheme3
    {
    public:
      weights3_t operator()(const std::string &codon1, const std::string &codon2,Sequence::GeneticCodes genetic_code) const;
    };
}
#endif

