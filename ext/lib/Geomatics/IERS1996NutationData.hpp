//==============================================================================
//
//  This file is part of GNSSTk, the ARL:UT GNSS Toolkit.
//
//  The GNSSTk is free software; you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published
//  by the Free Software Foundation; either version 3.0 of the License, or
//  any later version.
//
//  The GNSSTk is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with GNSSTk; if not, write to the Free Software Foundation,
//  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
//
//  This software was developed by Applied Research Laboratories at the
//  University of Texas at Austin.
//  Copyright 2004-2022, The Board of Regents of The University of Texas System
//
//==============================================================================

//==============================================================================
//
//  This software was developed by Applied Research Laboratories at the
//  University of Texas at Austin, under contract to an agency or agencies
//  within the U.S. Department of Defense. The U.S. Government retains all
//  rights to use, duplicate, distribute, disclose, or release this software.
//
//  Pursuant to DoD Directive 523024
//
//  DISTRIBUTION STATEMENT A: This software has been approved for public
//                            release, distribution is unlimited.
//
//==============================================================================

   // adapted from SOFA nut80.c

   static const struct {
      int nl,nlp,nf,nd,nom; // coefficients of l,l',F,D,Om
      // units 0.1mas and 0.1mas/JulianCentury
      double sp,spt;        // longitude sine and rate coefficients
      double ce,cet;        // obliquity cosine and rate coefficients
   } coeff[] = {

   // index+1 = 1-10
      {  0,  0,  0,  0,  1, -171996.0, -174.2,  92025.0,    8.9 },
      {  0,  0,  0,  0,  2,    2062.0,    0.2,   -895.0,    0.5 },
      { -2,  0,  2,  0,  1,      46.0,    0.0,    -24.0,    0.0 },
      {  2,  0, -2,  0,  0,      11.0,    0.0,      0.0,    0.0 },
      { -2,  0,  2,  0,  2,      -3.0,    0.0,      1.0,    0.0 },
      {  1, -1,  0, -1,  0,      -3.0,    0.0,      0.0,    0.0 },
      {  0, -2,  2, -2,  1,      -2.0,    0.0,      1.0,    0.0 },
      {  2,  0, -2,  0,  1,       1.0,    0.0,      0.0,    0.0 },
      {  0,  0,  2, -2,  2,  -13187.0,   -1.6,   5736.0,   -3.1 },
      {  0,  1,  0,  0,  0,    1426.0,   -3.4,     54.0,   -0.1 },
   // index+1 = 11-20
      {  0,  1,  2, -2,  2,    -517.0,    1.2,    224.0,   -0.6 },
      {  0, -1,  2, -2,  2,     217.0,   -0.5,    -95.0,    0.3 },
      {  0,  0,  2, -2,  1,     129.0,    0.1,    -70.0,    0.0 },
      {  2,  0,  0, -2,  0,      48.0,    0.0,      1.0,    0.0 },
      {  0,  0,  2, -2,  0,     -22.0,    0.0,      0.0,    0.0 },
      {  0,  2,  0,  0,  0,      17.0,   -0.1,      0.0,    0.0 },
      {  0,  1,  0,  0,  1,     -15.0,    0.0,      9.0,    0.0 },
      {  0,  2,  2, -2,  2,     -16.0,    0.1,      7.0,    0.0 },
      {  0, -1,  0,  0,  1,     -12.0,    0.0,      6.0,    0.0 },
      { -2,  0,  0,  2,  1,      -6.0,    0.0,      3.0,    0.0 },
   // index+1 = 21-30
      {  0, -1,  2, -2,  1,      -5.0,    0.0,      3.0,    0.0 },
      {  2,  0,  0, -2,  1,       4.0,    0.0,     -2.0,    0.0 },
      {  0,  1,  2, -2,  1,       4.0,    0.0,     -2.0,    0.0 },
      {  1,  0,  0, -1,  0,      -4.0,    0.0,      0.0,    0.0 },
      {  2,  1,  0, -2,  0,       1.0,    0.0,      0.0,    0.0 },
      {  0,  0, -2,  2,  1,       1.0,    0.0,      0.0,    0.0 },
      {  0,  1, -2,  2,  0,      -1.0,    0.0,      0.0,    0.0 },
      {  0,  1,  0,  0,  2,       1.0,    0.0,      0.0,    0.0 },
      { -1,  0,  0,  1,  1,       1.0,    0.0,      0.0,    0.0 },
      {  0,  1,  2, -2,  0,      -1.0,    0.0,      0.0,    0.0 },
   // index+1 = 31-40
      {  0,  0,  2,  0,  2,   -2274.0,   -0.2,    977.0,   -0.5 },
      {  1,  0,  0,  0,  0,     712.0,    0.1,     -7.0,    0.0 },
      {  0,  0,  2,  0,  1,    -386.0,   -0.4,    200.0,    0.0 },
      {  1,  0,  2,  0,  2,    -301.0,    0.0,    129.0,   -0.1 },
      {  1,  0,  0, -2,  0,    -158.0,    0.0,     -1.0,    0.0 },
      { -1,  0,  2,  0,  2,     123.0,    0.0,    -53.0,    0.0 },
      {  0,  0,  0,  2,  0,      63.0,    0.0,     -2.0,    0.0 },
      {  1,  0,  0,  0,  1,      63.0,    0.1,    -33.0,    0.0 },
      { -1,  0,  0,  0,  1,     -58.0,   -0.1,     32.0,    0.0 },
      { -1,  0,  2,  2,  2,     -59.0,    0.0,     26.0,    0.0 },
   // index+1 = 41-50
      {  1,  0,  2,  0,  1,     -51.0,    0.0,     27.0,    0.0 },
      {  0,  0,  2,  2,  2,     -38.0,    0.0,     16.0,    0.0 },
      {  2,  0,  0,  0,  0,      29.0,    0.0,     -1.0,    0.0 },
      {  1,  0,  2, -2,  2,      29.0,    0.0,    -12.0,    0.0 },
      {  2,  0,  2,  0,  2,     -31.0,    0.0,     13.0,    0.0 },
      {  0,  0,  2,  0,  0,      26.0,    0.0,     -1.0,    0.0 },
      { -1,  0,  2,  0,  1,      21.0,    0.0,    -10.0,    0.0 },
      { -1,  0,  0,  2,  1,      16.0,    0.0,     -8.0,    0.0 },
      {  1,  0,  0, -2,  1,     -13.0,    0.0,      7.0,    0.0 },
      { -1,  0,  2,  2,  1,     -10.0,    0.0,      5.0,    0.0 },
   // index+1 = 51-60
      {  1,  1,  0, -2,  0,      -7.0,    0.0,      0.0,    0.0 },
      {  0,  1,  2,  0,  2,       7.0,    0.0,     -3.0,    0.0 },
      {  0, -1,  2,  0,  2,      -7.0,    0.0,      3.0,    0.0 },
      {  1,  0,  2,  2,  2,      -8.0,    0.0,      3.0,    0.0 },
      {  1,  0,  0,  2,  0,       6.0,    0.0,      0.0,    0.0 },
      {  2,  0,  2, -2,  2,       6.0,    0.0,     -3.0,    0.0 },
      {  0,  0,  0,  2,  1,      -6.0,    0.0,      3.0,    0.0 },
      {  0,  0,  2,  2,  1,      -7.0,    0.0,      3.0,    0.0 },
      {  1,  0,  2, -2,  1,       6.0,    0.0,     -3.0,    0.0 },
      {  0,  0,  0, -2,  1,      -5.0,    0.0,      3.0,    0.0 },
   // index+1 = 61-70
      {  1, -1,  0,  0,  0,       5.0,    0.0,      0.0,    0.0 },
      {  2,  0,  2,  0,  1,      -5.0,    0.0,      3.0,    0.0 },
      {  0,  1,  0, -2,  0,      -4.0,    0.0,      0.0,    0.0 },
      {  1,  0, -2,  0,  0,       4.0,    0.0,      0.0,    0.0 },
      {  0,  0,  0,  1,  0,      -4.0,    0.0,      0.0,    0.0 },
      {  1,  1,  0,  0,  0,      -3.0,    0.0,      0.0,    0.0 },
      {  1,  0,  2,  0,  0,       3.0,    0.0,      0.0,    0.0 },
      {  1, -1,  2,  0,  2,      -3.0,    0.0,      1.0,    0.0 },
      { -1, -1,  2,  2,  2,      -3.0,    0.0,      1.0,    0.0 },
      { -2,  0,  0,  0,  1,      -2.0,    0.0,      1.0,    0.0 },
   // index+1 = 71-80
      {  3,  0,  2,  0,  2,      -3.0,    0.0,      1.0,    0.0 },
      {  0, -1,  2,  2,  2,      -3.0,    0.0,      1.0,    0.0 },
      {  1,  1,  2,  0,  2,       2.0,    0.0,     -1.0,    0.0 },
      { -1,  0,  2, -2,  1,      -2.0,    0.0,      1.0,    0.0 },
      {  2,  0,  0,  0,  1,       2.0,    0.0,     -1.0,    0.0 },
      {  1,  0,  0,  0,  2,      -2.0,    0.0,      1.0,    0.0 },
      {  3,  0,  0,  0,  0,       2.0,    0.0,      0.0,    0.0 },
      {  0,  0,  2,  1,  2,       2.0,    0.0,     -1.0,    0.0 },
      { -1,  0,  0,  0,  2,       1.0,    0.0,     -1.0,    0.0 },
      {  1,  0,  0, -4,  0,      -1.0,    0.0,      0.0,    0.0 },
   // index+1 = 81-90
      { -2,  0,  2,  2,  2,       1.0,    0.0,     -1.0,    0.0 },
      { -1,  0,  2,  4,  2,      -2.0,    0.0,      1.0,    0.0 },
      {  2,  0,  0, -4,  0,      -1.0,    0.0,      0.0,    0.0 },
      {  1,  1,  2, -2,  2,       1.0,    0.0,     -1.0,    0.0 },
      {  1,  0,  2,  2,  1,      -1.0,    0.0,      1.0,    0.0 },
      { -2,  0,  2,  4,  2,      -1.0,    0.0,      1.0,    0.0 },
      { -1,  0,  4,  0,  2,       1.0,    0.0,      0.0,    0.0 },
      {  1, -1,  0, -2,  0,       1.0,    0.0,      0.0,    0.0 },
      {  2,  0,  2, -2,  1,       1.0,    0.0,     -1.0,    0.0 },
      {  2,  0,  2,  2,  2,      -1.0,    0.0,      0.0,    0.0 },
   // index+1 = 91-100
      {  1,  0,  0,  2,  1,      -1.0,    0.0,      0.0,    0.0 },
      {  0,  0,  4, -2,  2,       1.0,    0.0,      0.0,    0.0 },
      {  3,  0,  2, -2,  2,       1.0,    0.0,      0.0,    0.0 },
      {  1,  0,  2, -2,  0,      -1.0,    0.0,      0.0,    0.0 },
      {  0,  1,  2,  0,  1,       1.0,    0.0,      0.0,    0.0 },
      { -1, -1,  0,  2,  1,       1.0,    0.0,      0.0,    0.0 },
      {  0,  0, -2,  0,  1,      -1.0,    0.0,      0.0,    0.0 },
      {  0,  0,  2, -1,  2,      -1.0,    0.0,      0.0,    0.0 },
      {  0,  1,  0,  2,  0,      -1.0,    0.0,      0.0,    0.0 },
      {  1,  0, -2, -2,  0,      -1.0,    0.0,      0.0,    0.0 },
   // index+1 = 101-106
      {  0, -1,  2,  0,  1,      -1.0,    0.0,      0.0,    0.0 },
      {  1,  1,  0, -2,  1,      -1.0,    0.0,      0.0,    0.0 },
      {  1,  0, -2,  2,  0,      -1.0,    0.0,      0.0,    0.0 },
      {  2,  0,  0,  2,  0,       1.0,    0.0,      0.0,    0.0 },
      {  0,  0,  2,  4,  2,      -1.0,    0.0,      0.0,    0.0 },
      {  0,  1,  0,  1,  0,       1.0,    0.0,      0.0,    0.0 }
   };

   // Number of terms in the series
   const int Ncoeff = (int) (sizeof coeff / sizeof coeff[0]);
