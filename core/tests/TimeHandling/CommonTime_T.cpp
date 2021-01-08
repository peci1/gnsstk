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
//  Copyright 2004-2020, The Board of Regents of The University of Texas System
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

#include "CommonTime.hpp"
#include "Exception.hpp"
#include "TestUtil.hpp"
#include "BasicTimeSystemConverter.hpp"
#include "CivilTime.hpp"
#include <iostream>
#include <cmath>
using namespace gpstk;
using namespace std;

class CommonTime_T : public CommonTime
{
public:

      /// Default Constructor, set the precision value
   CommonTime_T();

      /** Test to see if any of the standard assignment methods break
       * when using proper inputs. */
   unsigned initializationTest();

      /** Test to see if setting improper values induces the correct
       * exception handling. */
   unsigned improperSetTest();

      /** Test to check the various set methods are equivalent
       * when they should be. */
   unsigned setComparisonTest();

      /// Test to check arithmetic operations function properly
   unsigned arithmeticTest();

      /// Test the comparison operators
   unsigned operatorTest();

      /// Test will check the reset method.
   unsigned resetTest();

      /** Test will check the TimeSystem comparisons when using
       * the comparison operators. */
   unsigned timeSystemTest();

      /// Test Formatted Printing
   unsigned printfTest();

      /** Test to check arithmetic operations function properly when
       * rolling over or under the three time variables */
   unsigned rolloverTest();

   unsigned changeTimeSystemTest();
private:

   double eps;
};


CommonTime_T ::
CommonTime_T()
{
   eps = 1e-11;
}


unsigned CommonTime_T ::
initializationTest()
{
   TUDEF("CommonTime", "CommonTime");

      //----------------------------------------
      // Constructor test
      //----------------------------------------
   try
   {
      CommonTime Zero;
      TUPASS("CommonTime constructor did not throw an exception.");
   }
   catch(...)
   {
      TUFAIL("CommonTime constructor should not have thrown an exception.");
   }

      //----------------------------------------
      // CommonTime.set() test
      //----------------------------------------
   try
   {
      CommonTime Test1;
      Test1.set( (long)700000, (long)0, (double)0. );
      TUPASS("CommonTime.set() did not throw an exception.");
   }
   catch (...)
   {
      TUFAIL("CommonTime.set() threw an exception, but should not have.");
   }

      // Undocumented comment line:
      //     CommonTime Test1((long)700000,(long)0,(double)0.); // This is a protected method

      //----------------------------------------
      // Copy-Constructer test
      //----------------------------------------
   try
   {
      CommonTime Test1;
      Test1.set( (long)700000, (long)0, (double)0. );
      CommonTime Test2(Test1);
      TUPASS("CommonTime2(CommonTime1) copy constructor did not throw an exception.");
   }
   catch (...)
   {
      TUFAIL("CommonTime Copy Constructor threw an exception, but should not have");
   }

      //----------------------------------------
      // Assignment operator test
      //----------------------------------------
   try
   {
      CommonTime Test1;
      Test1.set( (long)700000, (long)0, (double)0. );
      CommonTime Test3 = Test1;
      TUPASS("CommonTime assignment operator did not throw an exception.");
   }
   catch (...)
   {
      TUFAIL("CommonTime assignment operator on same line threw an exception, but should not have");
   }

      //----------------------------------------
      // Assignment operator test
      //----------------------------------------
   try
   {
      CommonTime Test1;
      Test1.set( (long)700000, (long)0, (double)0.);
      CommonTime Test4;
      Test4 = Test1;
      TUPASS("CommonTime assignment operator did not throw an exception");
   }
   catch (...)
   {
      TUFAIL("CommonTime assignment operator on separate line should not have thrown an exception");
   }

      //----------------------------------------
      // The End!
      //----------------------------------------
   TURETURN();
}


unsigned CommonTime_T ::
improperSetTest()
{
   CommonTime Test;
   Test.set (700000, 0, 0.);

   TUDEF("CommonTime", "set");

      // Break the input in various ways and make sure the proper exception is called

      //----------------------------------------
      // Does CommonTime.set() work with negative days?
      //----------------------------------------
   try
   {
      Test.set(-1,0,0.);
      TUFAIL("[testing] CommonTime.set() with negative day, exception gpstk::Exception, [actual] threw no exception");
   }
      // QUESTION: Why test for gpstk::InvalidRequest exception instead of gpstk::Exception?
      // ANSWER: gpstk::InvalidRequest is a child of gpstk::Exception, and depending on CommonTime.cpp
      // we might sometimes need to be more specific than catching gpstk::Exception
      //     catch( gpstk::InvalidParameter e)
      // For now, I'm replacing gpstk::InvalidParameter with gpstk::Exception to be consistent throughout this test application
   catch( gpstk::Exception e)
   {
      TUPASS("CommonTime.set() with negative day should throw a gpstk::Exception");
   }
   catch(...)
   {
      TUFAIL("[testing] CommonTime.set() with negative day, exception gpstk::Exception, [actual] threw wrong exception");
   }

      //----------------------------------------
      // Does CommonTime.set() work with too many days?
      //----------------------------------------
   try
   {
      Test.set(3442449,0,0.);
      TUFAIL("[testing] CommonTime.set() with too many days, exception gpstk::Exception, [actual] threw no exception");
   }
   catch(gpstk::Exception e)
   {
      TUPASS("CommonTime.set() with too many days should throw a gpstk::Exception" );
   }
   catch (...)
   {
      TUFAIL("[testing] CommonTime.set() with too many days, exception gpstk::Exception, [actual] threw wrong exception");
   }

      //----------------------------------------
      // Does CommonTime.set() work with negative seconds?
      //----------------------------------------
   try
   {
      Test.set(700000,-1,0.);
      TUFAIL("[testing] CommonTime.set() with negative seconds, exception gpstk::Exception, [actual] threw no exception");
   }
   catch(gpstk::Exception e)
   {
      TUPASS("CommonTime.set() with negative seconds should throw a gpstk::Exception");
   }
   catch (...)
   {
      TUFAIL("Fail");
   }

      //----------------------------------------
      // Does a set method work with too many seconds?
      //----------------------------------------
   try
   {
      Test.set(700000,24*60*60+1,0.);
      TUFAIL("[testing] CommonTime.set() with too many seconds, exception gpstk::Exception, [actual] threw no exception");
   }
   catch(gpstk::Exception e)
   {
      TUPASS("CommonTime.set() with too many seconds should throw a gpstk::Exception");
   }
   catch (...)
   {
      TUFAIL("[testing] CommonTime.set() with too many seconds, exception gpstk::Exception, [actual] threw wrong exception");
   }

      //----------------------------------------
      // Does a set method work with negative fractional seconds?
      //----------------------------------------
   try
   {
      Test.set(700000,0,-1.);
      TUFAIL("[testing] CommonTime.set() with negative fractional seconds, exception gpstk::Exception, [actual] threw no exception");
   }
   catch(gpstk::Exception e)
   {
      TUPASS("CommonTime.set() with negative fractional seconds should throw a gpstk::Exception");
   }
   catch (...)
   {
      TUFAIL("[testing] CommonTime.set() with negative fractional seconds, exception gpstk::Exception, [actual] threw wrong exception");
   }

      //----------------------------------------
      // Does a set method work with too many fractional seconds?
      //----------------------------------------
   try
   {
      Test.set(700000,0,2.);
      TUFAIL("[testing] CommonTime.set() with too many fractional seconds, exception gpstk::Exception, [actual] threw no exception");
   }
   catch( gpstk::Exception e )
   {
      TUPASS("CommonTime.set() with too many fractional seconds should throw a gpstk::Exception");
   }
   catch(...)
   {
      TUFAIL("[testing] CommonTime.set() with too many fractional seconds, exception gpstk::Exception, [actual] threw wrong exception");
   }

      //----------------------------------------
      // Does a setInternal work with negative days?
      //----------------------------------------
   try
   {
      Test.setInternal(-1,0,0.);
      TUFAIL("[testing] CommonTime.setInternal() with negative days, exception gpstk::Exception, [actual] threw no exception");
   }
   catch( gpstk::Exception e )
   {
      TUPASS("CommonTime.setInternal() with negative days should throw a gpstk::Exception");
   }
   catch (...)
   {
      TUFAIL("[testing] CommonTime.setInternal() with negative days, exception gpstk::Exception, [actual] threw wrong exception");
   }

      //----------------------------------------
      // Does a setInternal work with too many days?
      //----------------------------------------
   try
   {
      Test.setInternal(3442449,0,0.);
      TUFAIL("[testing] CommonTime.setInternal() with too many days, exception gpstk::Exception, [actual] threw no exception");
   }
   catch(gpstk::Exception e)
   {
      TUPASS("CommonTime.setInternal() with too many days should throw a gpstk::Exception");
   }
   catch (...)
   {
      TUFAIL("[testing] CommonTime.setInternal() with too many days, exception gpstk::Exception, [actual] threw wrong exception");
   }

      //----------------------------------------
      // Does a setInternal work with negative milliseconds?
      //----------------------------------------
   try
   {
      Test.setInternal(700000,-1,0.);
      TUFAIL("[testing] CommonTime.setInternal() with negative milliseconds, exception gpstk::Exception, [actual] threw no exception");
   }
   catch(gpstk::Exception e)
   {
      TUPASS("CommonTime.setInternal() with negative milliseconds should throw a gpstk::Exception");
   }
   catch (...)
   {
      TUFAIL("[testing] CommonTime.setInternal() with negative milliseconds, exception gpstk::Exception, [actual] threw wrong exception");
   }

      //----------------------------------------
      // Does a setInternal method work with too many milliseconds?
      //----------------------------------------
   try
   {
      Test.setInternal(700000,24*60*60*1000+1,0.);
      TUFAIL("[testing] CommonTime.setInternal() with too many milliseconds, exception gpstk::Exception, [actual] threw no exception");
   }
   catch(gpstk::Exception e)
   {
      TUPASS("CommonTime.setInternal() with too many milliseconds should throw a gpstk::Exception");
   }
      // catch(...)
      // {
      //     TUFAIL("[testing] CommonTime.setInternal() with too many milliseconds, exception gpstk::Exception, [actual] threw wrong exception");
      // }

      //----------------------------------------
      // Does a setInternal method work with negative fractional seconds?
      //----------------------------------------
   try
   {
      Test.setInternal(700000,1001,-1.);
      TUFAIL("[testing] CommonTime.setInternal() with negative fractional seconds, exception gpstk::Exception, [actual] threw no exception");
   }
   catch(gpstk::Exception e)
   {
      TUPASS("CommonTime.setInternal() with negative fractional seconds should throw a gpstk::Exception");
   }
   catch (...)
   {
      TUFAIL("[testing] CommonTime.setInternal() with negative fractional seconds, exception gpstk::Exception, [actual] threw wrong exception");
   }

      //----------------------------------------
      // Does a setInternal method work with too many fractional seconds?
      //----------------------------------------
   try
   {
      Test.setInternal(700000,1001,1001.);
      TUFAIL("[testing] CommonTime.setInternal() with too many fractional seconds, exception gpstk::Exception, [actual] threw no exception");
   }
   catch(gpstk::Exception e)
   {
      TUPASS("CommonTime.setInternal() with too many fractional seconds should throw a gpstk::Exception");
   }
   catch(...)
   {
      TUFAIL("[testing] CommonTime.setInternal() with too many fractional seconds, exception gpstk::Exception, [actual] threw wrong exception");
   }

      //----------------------------------------
      // The End!
      //----------------------------------------
   TURETURN();
}


unsigned CommonTime_T ::
setComparisonTest()
{
   TUDEF("CommonTime", "set");

   CommonTime Test1, Test2, Test3, Test4;
   long day, day2;
   long sod, sod2;
   double fsod, fsod2;
   double dec = 1.1/SEC_PER_DAY;

      //----------------------------------------
      // Set in different ways
      //----------------------------------------
   Test1.set(700001,1,.1);             //Set with set(long day,long sod,double fsod,TimeSystem timeSystem = TimeSystem::Unknown )
   Test2.set(700001,1.1);              //Set with set(long day,double sod,TimeSystem timeSystem = TimeSystem::Unknown )
   Test3.set(700001 + dec);            //Set with set(double day,TimeSystem timeSys = TimeSystem::Unknown )
   Test4.setInternal(700001,1100,0.);  //Set with setInternal(long day,long msod,double fsod,TimeSystem timeSys = TimeSystem::Unknown );

      //Load up compare variables
   Test1.get(day,sod,fsod);
   Test2.get(day2,sod2,fsod2);

   TUASSERTE(long, day, day2);
   TUASSERTE(long, sod, sod2);
   TUASSERTFEPS(fsod, fsod2, eps);

      //----------------------------------------
      // Load up compare variables
      //----------------------------------------
   Test3.get( day2, sod2, fsod2 );

   TUASSERTE(long, day, day2);
   TUASSERTE(long, sod, sod2);

      //Testing results show fsod = 0.1 fsod2 = 0.100004
      //Appears to be a result of the input double is 700001.000012732
      //Rounding the last digit appears to be causing the issue and the
      //large error.

   testFramework.assert( fabs(fsod - fsod2) < 1E-4, "Does a set method store the correct fsod value?", __LINE__ );

      //----------------------------------------
      // Adding a test for a much lower day value to ensure the
      // error is from round off error.
      //----------------------------------------
   Test1.set(1,1,.1);
   Test3.set(1+dec);
   Test1.get(day,sod,fsod);
   Test3.get(day2,sod2,fsod2);

   testFramework.assert( fabs(fsod - fsod2) < eps, "Does a set method store the correct fsod value?", __LINE__ );

      //----------------------------------------
      //Load up compare variables
      //----------------------------------------
   Test1.set(700001,1,.1);
   Test1.get(day,sod,fsod);
   Test4.get(day2,sod2,fsod2);

   testFramework.assert( day == day2,              "Does a setInternal method store the correct day value?", __LINE__ );
   testFramework.assert( sod == sod2,              "Does a setInternal method store the correct sod value?", __LINE__ );
   testFramework.assert( fabs(fsod - fsod2) < eps, "Does a setInternal method store the correct sod value?", __LINE__ );

      //----------------------------------------
      // The End!
      //----------------------------------------
   TURETURN();
}


unsigned CommonTime_T ::
arithmeticTest()
{
   TUDEF("CommonTime", "Operators");


   CommonTime Arith1;
   Arith1.set(700000,1,0.1);
   CommonTime Arith2(Arith1); //Set second time equal to the first
   CommonTime Result;
   long day, day2, sod;
   double fsod, sod2;

   testFramework.assert( fabs((Arith1-Arith2) - 0) < eps, "Does it subtract between two CommonTime objects?", __LINE__ );


      //----------------------------------------
      // Add seconds with +
      //----------------------------------------
   Result = Arith2 + 1;
   Result.get(day,sod,fsod);
   testFramework.assert( day == 700000,          "Does it not add to the day value?",  __LINE__ );
   testFramework.assert( sod == 2,               "Does it add to the sod value?",      __LINE__ );
   testFramework.assert( fabs(fsod - 0.1) < eps, "Does it not add to the fsod value?", __LINE__ );

      //----------------------------------------
      // Subtract seconds with -
      //----------------------------------------
   Result = Arith2 - 1;
   Result.get(day,sod,fsod);
   testFramework.assert( day == 700000,          "Does it not subtract from the day value?",  __LINE__ );
   testFramework.assert( sod == 0,               "Does it subtract from the sod value?",      __LINE__ );
   testFramework.assert( fabs(fsod - 0.1) < eps, "Does it not subtract from the fsod value?", __LINE__ );

      //----------------------------------------
      // Add seconds with +=
      //----------------------------------------
   Arith2 += 1;
   testFramework.assert( fabs((Arith2-Arith1) - 1) < eps, "Does it add to a CommonTime object?",                    __LINE__ );
   testFramework.assert( 1 == Arith2 - Arith1,            "Check that values can be compared with integer seconds", __LINE__ );

      //----------------------------------------
      // Subtract seconds with -=
      //----------------------------------------
   Arith2 -= 1;
   testFramework.assert(fabs((Arith2-Arith1) - 0) < eps, "Does it subtract from a CommonTime object?", __LINE__ );

      //----------------------------------------
      // Add days with addDays
      //----------------------------------------
   Arith2.addDays((long)1);
   day = Arith2.getDays();
   testFramework.assert( 700001. == day, "Does the addDays method function correctly with +?", __LINE__ );

      // Subtract days with addDays
   Arith2.addDays((long)-1);
   day = Arith2.getDays();
   testFramework.assert( 700000. == day, "Does the addDays method function correctly with -?", __LINE__ );

      // Add seconds with addSeconds(double)
   Arith2.addSeconds(86400000.+1000.);
   testFramework.assert(fabs(86401000. - (Arith2-Arith1)) < eps, "Does the addSeconds method function correctly with +?", __LINE__ );

      // Subtract seconds with addSeconds(long)
   Arith2.addSeconds((long)-86401000);
   testFramework.assert( fabs(0. - (Arith2-Arith1)) < eps, "Does the addSeconds method function correctly with -?", __LINE__ );

      // Check that the two parameter get method returns day2 as the proper double
   Arith2.get(day2, sod2);
   testFramework.assert( (long)700000 == day2,      "Does the 2 parameter get method reuturn days as a double?", __LINE__ );
   testFramework.assert( ((double)0. - sod2) < eps, "Does the 2 parameter get method reuturn days as a double?", __LINE__ );

      // Check seconds using getSecondOfDay()
   testFramework.assert( fabs(sod2 - Arith2.getSecondOfDay()) < eps, "Check seconds using getSecondOfDay()", __LINE__ );

      // Add milliseconds with addMilliseconds(long)
   Arith2.addMilliseconds( (long)1 );
   testFramework.assert( fabs(sod2+0.001 - Arith2.getSecondOfDay()) < eps, "Does the addMilliseconds method function correctly with +?", __LINE__ );

   Arith2.addMilliseconds( (long)-1 );
   testFramework.assert(fabs(sod2 - Arith2.getSecondOfDay()) < eps, "Does the addMilliseconds method function correctly with -?", __LINE__ );

      //----------------------------------------
      // The End!
      //----------------------------------------
   TURETURN();
}


unsigned CommonTime_T ::
operatorTest()
{
   TUDEF("CommonTime", "Differing TimeSystem, Operator ==");

   CommonTime Compare; Compare.set(1000,200,0.2); // Initialize with value
   CommonTime LessThanDay; LessThanDay.set(100,200,0.2); // Initialize with smaller day value
   CommonTime LessThanSecond; LessThanSecond.set(1000,20,0.2); // Initialize with smaller second value
   CommonTime LessThanFSecond; LessThanFSecond.set(1000,200,0.1); // Initialize with smaller fractional second value
   CommonTime CompareCopy(Compare); // Initialize with copy constructor

   testFramework.assert( Compare == CompareCopy,    "GPSWeekZCount operator ==, Are equivalent objects equivalent?",     __LINE__ );
   testFramework.assert( !(Compare == LessThanDay), "GPSWeekZCount operator !=, Are non-equivalent objects equivalent?", __LINE__ );

      //----------------------------------------
      // Operator !=
      //----------------------------------------
   testFramework.changeSourceMethod( "Operator !=" );
   testFramework.assert( Compare != LessThanDay,     "GPSWeekZCount operator !=, Are non-equivalent objects not equivalent?", __LINE__ );
   testFramework.assert( Compare != LessThanSecond,  "GPSWeekZCount operator !=, Are non-equivalent objects not equivalent?", __LINE__ );
   testFramework.assert( Compare != LessThanFSecond, "GPSWeekZCount operator !=, Are non-equivalent objects not equivalent?", __LINE__ );
   testFramework.assert( !(Compare != Compare),      "GPSWeekZCount operator !=, Are equivalent objects not equivalent?",     __LINE__ );

      //----------------------------------------
      // Operator <
      //----------------------------------------
   testFramework.changeSourceMethod( "Operator <" );
   testFramework.assert( LessThanDay < Compare,        "Does the < operator function when left_object < right_object?",            __LINE__ );
   testFramework.assert( LessThanSecond < Compare,     "Does the < operator function when left_object < right_object by days?",    __LINE__ );
   testFramework.assert( !(Compare < LessThanSecond),  "Does the < operator function when left_object > right_object by days?",    __LINE__ );
   testFramework.assert( LessThanFSecond < Compare,    "Does the < operator function when left_object < right_object by seconds?", __LINE__ );
   testFramework.assert( !(Compare < LessThanFSecond), "Does the < operator function when left_object > right_object by seconds?", __LINE__ );
   testFramework.assert( !(Compare < CompareCopy),     "Does the < operator function when left_object = right_object?",            __LINE__ );

      //----------------------------------------
      // Greater than assertions
      //----------------------------------------
   testFramework.changeSourceMethod( "Operator >" );
   testFramework.assert( Compare > LessThanDay,        "Does the > operator function when left_object > right_object by years?",   __LINE__ );
   testFramework.assert( !(LessThanDay > Compare),     "Does the > operator function when left_object < right_object by years?",   __LINE__ );
   testFramework.assert( Compare > LessThanSecond,     "Does the > operator function when left_object > right_object by days?",    __LINE__ );
   testFramework.assert( !(LessThanSecond > Compare),  "Does the > operator function when left_object < right_object by days?",    __LINE__ );
   testFramework.assert( Compare > LessThanFSecond,    "Does the > operator function when left_object > right_object by seconds?", __LINE__ );
   testFramework.assert( !(LessThanFSecond > Compare), "Does the > operator function when left_object < right_object by seconds?", __LINE__ );
   testFramework.assert( !(Compare > CompareCopy),     "Does the > operator function when left_object = right_object?",            __LINE__ );

      //----------------------------------------
      // Less than equals assertion
      //----------------------------------------
   testFramework.changeSourceMethod( "Operator <=" );
   testFramework.assert( LessThanDay <= Compare,        "Does the < operator function when left_object < right_object by years?",    __LINE__ );
   testFramework.assert( !(Compare <= LessThanDay),     "Does the <= operator function when left_object > right_object by years?",   __LINE__ );
   testFramework.assert( LessThanSecond <= Compare,     "Does the <= operator function when left_object < right_object by days?",    __LINE__ );
   testFramework.assert( !(Compare <= LessThanSecond),  "Does the <= operator function when left_object > right_object by days?",    __LINE__ );
   testFramework.assert( LessThanFSecond <= Compare,    "Does the <= operator function when left_object < right_object by seconds?", __LINE__ );
   testFramework.assert( !(Compare <= LessThanFSecond), "Does the <= operator function when left_object > right_object by seconds?", __LINE__ );
   testFramework.assert( Compare <= CompareCopy,        "Does the <= operator function when left_object = right_object?",            __LINE__ );

      //----------------------------------------
      // Greater than equals assertion
      //----------------------------------------
   testFramework.changeSourceMethod( "Operator >=" );
   testFramework.assert( Compare >= LessThanDay,        "Does the >= operator function when left_object > right_object by years?",   __LINE__ );
   testFramework.assert( !(LessThanDay >= Compare),     "Does the >= operator function when left_object < right_object by years?",   __LINE__ );
   testFramework.assert( Compare >= LessThanSecond,     "Does the >= operator function when left_object > right_object by days?",    __LINE__ );
   testFramework.assert( !(LessThanSecond >= Compare),  "Does the >= operator function when left_object < right_object by days?",    __LINE__ );
   testFramework.assert( Compare >= LessThanFSecond,    "Does the >= operator function when left_object > right_object by seconds?", __LINE__ );
   testFramework.assert( !(LessThanFSecond >= Compare), "Does the >= operator function when left_object < right_object by seconds?", __LINE__ );
      // typo from last guy??
   testFramework.assert( !(Compare < CompareCopy),      "Does the >  operator function when left_object = right_object?",            __LINE__ );

      //----------------------------------------
      // The End!
      //----------------------------------------
   TURETURN();
}


unsigned CommonTime_T ::
resetTest()
{
   TUDEF("CommonTime", "reset" );

   CommonTime Compare; Compare.set(1000,200,0.2); // Initialize with value
   long day, sod;
   double fsod;
   Compare.reset(); // Reset it
   Compare.get(day,sod,fsod);

   testFramework.assert( TimeSystem(0) == Compare.getTimeSystem(), "Was the time system reset to expectation?", __LINE__  );
   testFramework.assert( 0 == day,  "Was the day value reset to expectation?",  __LINE__ );
   testFramework.assert( 0 == sod,  "Was the sod value reset to expectation?",  __LINE__ );
   testFramework.assert( 0 == fsod, "Was the fsod value reset to expectation?", __LINE__ );

   TURETURN();
}


unsigned CommonTime_T ::
timeSystemTest()
{
   TUDEF("CommonTime", "Differing TimeSystem == Operator");

   CommonTime GPS1;       GPS1.set( 1000, 200, 0.2, TimeSystem(2) );
   CommonTime GPS2;       GPS2.set( 100,  200, 0.2, TimeSystem(2) );
   CommonTime UTC1;       UTC1.set( 1000, 200, 0.2, TimeSystem(5) );
   CommonTime UNKNOWN; UNKNOWN.set( 1000, 200, 0.2, TimeSystem(0) );
   CommonTime ANY;         ANY.set( 1000, 200, 0.2, TimeSystem(1) );

      //----------------------------------------
      // ???
      //----------------------------------------
   testFramework.assert( !(GPS1 == GPS2), "Verify same Time System but different time inequality", __LINE__ );
   testFramework.assert( GPS1.getTimeSystem() == GPS2.getTimeSystem(), "Verify same Time System equality", __LINE__ );

      //----------------------------------------
      // Differing TimeSystem != Operator
      //----------------------------------------
   testFramework.changeSourceMethod( "Differing TimeSystem != Operator" );
   testFramework.assert( GPS1 != UTC1,    "Verify different Time System but same time inequality", __LINE__ );
   testFramework.assert( GPS1 != UNKNOWN, "Verify different Time System but same time inequality", __LINE__ );

      //----------------------------------------
      // ANY TimeSystem == Operator
      //----------------------------------------
   testFramework.changeSourceMethod( "ANY TimeSystem == Operator" );
   testFramework.assert( GPS1 == ANY,    "Verify TimeSystem=ANY does not matter in TimeSystem=GPS comparisons",    __LINE__ );
   testFramework.assert( UTC1 == ANY,    "Verify TimeSystem=ANY does not matter in TimeSystem=UTC comparisons",    __LINE__ );
   testFramework.assert( UNKNOWN == ANY, "Verify TimeSystem=ANY does not matter in TimeSystem=UNKOWN comparisons", __LINE__ );

      //----------------------------------------
      // ANY TimeSystem < Operator
      //----------------------------------------
   testFramework.changeSourceMethod( "ANY TimeSystem < Operator" );
   testFramework.assert( !(GPS2 == ANY) && (GPS2 < ANY), "Verify TimeSystem=ANY does not matter in other operator comparisons", __LINE__ );

      //----------------------------------------
      // setTimeSystem
      //----------------------------------------
   testFramework.changeSourceMethod( "setTimeSystem" );
   UNKNOWN.setTimeSystem( TimeSystem(2) ); //Set the Unknown TimeSystem
   testFramework.assert( UNKNOWN.getTimeSystem()==TimeSystem(2), "Ensure resetting a Time System changes it", __LINE__ );

      //----------------------------------------
      // The End!
      //----------------------------------------
   TURETURN();
}


unsigned CommonTime_T ::
printfTest()
{
   TUDEF("CommonTime", "printf");

   CommonTime GPS1; GPS1.set( 1234567, 24000, 0.2111, TimeSystem::GPS );
   CommonTime UTC1; UTC1.set( 1000,    200,   0.2,    TimeSystem::UTC );

   testFramework.assert( GPS1.asString() == (std::string)"1234567 24000211 0.000100000000000 GPS",             "Verify printed output matches expectation",       __LINE__ );
   testFramework.assert( UTC1.asString() == (std::string)"0001000 00200200 0.000000000000000 UTC",              "Verify printed output matches expectation",      __LINE__ );
   testFramework.assert( BEGINNING_OF_TIME.asString() == (std::string)"0000000 00000000 0.000000000000000 Any", "Test if BEGINNING_OF_TIME matches expectations", __LINE__ );

      //----------------------------------------
      // The End!
      //----------------------------------------
   TURETURN();
}


unsigned CommonTime_T ::
rolloverTest()
{
   TUDEF("CommonTime", "addSeconds");

   CommonTime  fsodRollover;  fsodRollover.set(10 , 6789 , 0.000999);
   CommonTime  msodRollover;  msodRollover.set(10 , 86399, 0.0001  );
   CommonTime  dayRollunder;  dayRollunder.set(10 , 2    , 0.0001  );
   CommonTime msodRollunder; msodRollunder.set(10 , 10   , 0.000001);

   CommonTime  expectedfsodROver;  expectedfsodROver.set(10, 6789 , 0.001000);
   CommonTime  expectedmsodROver;  expectedmsodROver.set(11, 0    , 0.0001);
   CommonTime  expectedDayRUnder;  expectedDayRUnder.set( 9, 86399, 0.0001); 
   CommonTime expectedmsodRUnder; expectedmsodRUnder.set(10, 9    , 0.999999);

   long    obtainedDay,  expectedDay;
   long   obtainedMsod, expectedMsod;
   double obtainedFsod, expectedFsod;
   double diff;
   long   incrementSecLong   = 1L      , decrementSecLong   = -3L;
   double incrementSecDouble = 0.000001, decrementSecDouble = -0.000002;

      //--------------------------------
      //Rollover Tests
      //--------------------------------

      //fsod Rollover test
   fsodRollover.addSeconds(incrementSecDouble);
   fsodRollover.get(obtainedDay,obtainedMsod,obtainedFsod);
   expectedfsodROver.get(expectedDay,expectedMsod,expectedFsod);

   testFramework.assert(obtainedDay == expectedDay  , "Rollover of fsod affected day value" , __LINE__);
   testFramework.assert(obtainedMsod == expectedMsod, "Rollover of fsod did not change msod", __LINE__);
   diff = fabs(obtainedFsod - expectedFsod);
   testFramework.assert(diff < eps, "fsod did not rollover properly"      , __LINE__);
 

      //msod Rollover test
   msodRollover.addSeconds(incrementSecLong);
   msodRollover.get(obtainedDay,obtainedMsod,obtainedFsod);
   expectedmsodROver.get(expectedDay,expectedMsod,expectedFsod);

   testFramework.assert(obtainedDay == expectedDay  , "Rollover of msod did not change day" , __LINE__);
   testFramework.assert(obtainedMsod == expectedMsod, "msod did not rollover properly"       , __LINE__);
   diff = fabs(obtainedFsod - expectedFsod);
   testFramework.assert(diff < eps, "Rollover of msod affected fsod oddly", __LINE__);


      //--------------------------------
      //Rollunder Tests
      //--------------------------------

      //fsod Rollover test
   dayRollunder.addSeconds(decrementSecLong);
   dayRollunder.get(obtainedDay,obtainedMsod,obtainedFsod);
   expectedDayRUnder.get(expectedDay,expectedMsod,expectedFsod);

   testFramework.assert(obtainedDay == expectedDay  , "Rollunder of msod did not change day" , __LINE__);
   testFramework.assert(obtainedMsod == expectedMsod, "msod did not rollunder properly"       , __LINE__);
   diff = fabs(obtainedFsod - expectedFsod);
   testFramework.assert(diff < eps, "Rollunder of msod affected fsod oddly", __LINE__);
 

      //msod Rollover test
   msodRollunder.addSeconds(decrementSecDouble);
   msodRollunder.get(obtainedDay,obtainedMsod,obtainedFsod);
   expectedmsodRUnder.get(expectedDay,expectedMsod,expectedFsod);

   testFramework.assert(obtainedDay == expectedDay  , "Rollunder of fsod affected day value" , __LINE__);
   testFramework.assert(obtainedMsod == expectedMsod, "Rollunder of fsod did not change msod", __LINE__);
   diff = fabs(obtainedFsod - expectedFsod);
   testFramework.assert(diff < eps, "fsod did not rollunder properly"      , __LINE__);

   TURETURN();
}


unsigned CommonTime_T ::
changeTimeSystemTest()
{
   TUDEF("CommonTime", "changeTimeSystem");
   gpstk::BasicTimeSystemConverter btsc;
   gpstk::CommonTime uut, exp;

      //Check conversion from any given time system to UTC and back 
   uut = gpstk::CivilTime(1990,11,6,0,0,0,gpstk::TimeSystem::UTC);
   exp = gpstk::CivilTime(1990,11,6,0,0,6,gpstk::TimeSystem::GPS);
   TUASSERTE(bool, true, uut.changeTimeSystem(gpstk::TimeSystem::GPS,&btsc));
   TUASSERTE(gpstk::CommonTime, uut, exp);

   uut = gpstk::CivilTime(2004,11,16,0,0,0,gpstk::TimeSystem::GPS);
   exp = gpstk::CivilTime(2004,11,15,23,59,47,gpstk::TimeSystem::UTC);
   TUASSERTE(bool, true, uut.changeTimeSystem(gpstk::TimeSystem::UTC,&btsc));
   TUASSERTE(gpstk::CommonTime, uut, exp);

   uut = gpstk::CivilTime(1992,10,3,0,0,0,gpstk::TimeSystem::UTC);
   exp = gpstk::CivilTime(1992,10,3,0,0,0,gpstk::TimeSystem::GLO);
   TUASSERTE(bool, true, uut.changeTimeSystem(gpstk::TimeSystem::GLO,&btsc));
   TUASSERTE(gpstk::CommonTime, uut, exp);

   uut = gpstk::CivilTime(1995,5,10,0,0,0,gpstk::TimeSystem::GLO);
   exp = gpstk::CivilTime(1995,5,10,0,0,0,gpstk::TimeSystem::UTC);
   TUASSERTE(bool, true, uut.changeTimeSystem(gpstk::TimeSystem::UTC,&btsc));
   TUASSERTE(gpstk::CommonTime, uut, exp);

   uut = gpstk::CivilTime(1995,5,10,0,0,0,gpstk::TimeSystem::GLO);
   exp = gpstk::CivilTime(1995,5,10,0,0,0,gpstk::TimeSystem::GLO);
   TUASSERTE(bool, true, uut.changeTimeSystem(gpstk::TimeSystem::GLO,&btsc));
   TUASSERTE(gpstk::CommonTime, uut, exp);

   uut = gpstk::CivilTime(2020,1,1,0,0,0,gpstk::TimeSystem::GPS);
   exp = gpstk::CivilTime(2019,12,31,23,59,42,gpstk::TimeSystem::GLO);
   TUASSERTE(bool, true, uut.changeTimeSystem(gpstk::TimeSystem::GLO,&btsc));
   TUASSERTE(gpstk::CommonTime, uut, exp);

   TURETURN();
}



//============================================================
// Main function to initialize and run all tests above
//============================================================
int main()
{
   unsigned errorTotal = 0;
   CommonTime_T testClass;

      //----------------------------------------
      // Run all test methods
      //----------------------------------------

   errorTotal += testClass.initializationTest();
   errorTotal += testClass.improperSetTest();
   errorTotal += testClass.setComparisonTest();
   errorTotal += testClass.arithmeticTest();
   errorTotal += testClass.rolloverTest();
   errorTotal += testClass.operatorTest();
   errorTotal += testClass.resetTest();
   errorTotal += testClass.timeSystemTest();
   errorTotal += testClass.printfTest();
   errorTotal += testClass.changeTimeSystemTest();

      //----------------------------------------
      // Echo total fails to stdout
      //----------------------------------------
   cout << "Total Failures for " << __FILE__ << ": " << errorTotal << endl;

      //----------------------------------------
      // Return total fails
      //----------------------------------------
   return errorTotal;
}
