//==============================================================================
//
//  This file is part of GPSTk, the GPS Toolkit.
//
//  The GPSTk is free software; you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published
//  by the Free Software Foundation; either version 3.0 of the License, or
//  any later version.
//
//  The GPSTk is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with GPSTk; if not, write to the Free Software Foundation,
//  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
//  
//  This software was developed by Applied Research Laboratories at the 
//  University of Texas at Austin.
//  Copyright 2004-2021, The Board of Regents of The University of Texas System
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

#ifndef GPSTK_TEST_FACTORYCOUNTER_HPP
#define GPSTK_TEST_FACTORYCOUNTER_HPP

#include "NavData.hpp"
#include "TestUtil.hpp"

// similar to TUASSERTE except requires a source line and counted data
// type name to be specified
#define FCASSERTE(TYPE,COUNTER,EXP,GOT,LINE)                            \
   try                                                                  \
   {                                                                    \
      std::ostringstream ostr;                                          \
      ostr << std::boolalpha << "Expected " << COUNTER << "=" << EXP    \
           << ", but got " << COUNTER << "=" << GOT;                    \
      testFramework.assert_equals<TYPE>(EXP, GOT, LINE, ostr.str());    \
   }                                                                    \
   catch (gpstk::Exception &exc)                                        \
   {                                                                    \
      std::cerr << exc << std::endl;                                    \
      testFramework.assert(false,                                       \
                           "Exception evaluating " #EXP " or " #GOT,    \
                           LINE);                                       \
   }                                                                    \
   catch (...)                                                          \
   {                                                                    \
      testFramework.assert(false,                                       \
                           "Exception evaluating " #EXP " or " #GOT,    \
                           LINE);                                       \
   }

/** Class for counting instances of various types of nav messages
 * produced by PNBNavDataFactory objects and the like. */
template <class Alm, class Eph, class TimeOffs, class Health, class Iono,
          class GrpDelay>
class FactoryCounter
{
public:
   FactoryCounter(gpstk::TestUtil& tf)
         : testFramework(tf)
   { resetCount(); }


   void resetCount()
   {
      almCount = 0;
      ephCount = 0;
      toCount = 0;
      heaCount = 0;
      ionoCount = 0;
      iscCount = 0;
      otherCount = 0;
   }


   void countResults(const gpstk::NavDataPtrList& navOut)
   {
      resetCount();
      for (const auto& i : navOut)
      {
         if (dynamic_cast<Alm*>(i.get()) != nullptr)
         {
            almCount++;
         }
         else if (dynamic_cast<Eph*>(i.get()) != nullptr)
         {
            ephCount++;
         }
         else if (dynamic_cast<TimeOffs*>(i.get()) != nullptr)
         {
            toCount++;
         }
         else if (dynamic_cast<Health*>(i.get()) != nullptr)
         {
            heaCount++;
         }
         else if (dynamic_cast<Iono*>(i.get()) != nullptr)
         {
            ionoCount++;
         }
         else if (dynamic_cast<GrpDelay*>(i.get()) != nullptr)
         {
            iscCount++;
         }
         else
         {
            otherCount++;
         }
      }
   }


   void validateResults(gpstk::NavDataPtrList& navOut,
                        unsigned lineNo,
                        size_t totalExp = 0,
                        unsigned almExp = 0,
                        unsigned ephExp = 0,
                        unsigned toExp = 0,
                        unsigned heaExp = 0,
                        unsigned ionoExp = 0,
                        unsigned iscExp = 0,
                        unsigned otherExp = 0)
   {
      countResults(navOut);
      FCASSERTE(size_t, "total", totalExp, navOut.size(), lineNo);
      FCASSERTE(unsigned, "almCount", almExp, almCount, lineNo);
      FCASSERTE(unsigned, "ephCount", ephExp, ephCount, lineNo);
      FCASSERTE(unsigned, "toCount", toExp, toCount, lineNo);
      FCASSERTE(unsigned, "heaCount", heaExp, heaCount, lineNo);
      FCASSERTE(unsigned, "ionoCount", ionoExp, ionoCount, lineNo);
      FCASSERTE(unsigned, "iscCount", iscExp, iscCount, lineNo);
      FCASSERTE(unsigned, "otherCount", otherExp, otherCount, lineNo);
      FCASSERTE(unsigned, "summed total", totalExp,
                almExp+ephExp+toExp+heaExp+ionoExp+iscExp+otherExp, lineNo);
      navOut.clear();
   }

      /// TestUtil object to use when assertions are integrated.
   gpstk::TestUtil& testFramework;
      /// Counts of messages, set by countResults.
   unsigned almCount, ephCount, toCount, heaCount, ionoCount, iscCount,
      otherCount;
};

#endif // GPSTK_TEST_FACTORYCOUNTER_HPP
