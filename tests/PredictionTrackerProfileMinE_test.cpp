
#include "catch.hpp"

#undef NDEBUG

#include "PredictionTrackerProfileMinE.h"
#include "AccessibilityDisabled.h"
#include "ReverseAccessibility.h"
#include "InteractionEnergyBasePair.h"

#include <stdexcept>


TEST_CASE( "PredictionTrackerProfileMinE", "[PredictionTrackerProfileMinE]" ) {

	// setup dummy data
	RnaSequence r1("r1","AACCG");
	RnaSequence r2("r2","AGUUNNNN");
	AccessibilityDisabled acc1(r1, 0, NULL);
	AccessibilityDisabled acc2(r2, 0, NULL);
	ReverseAccessibility racc( acc2 );
	InteractionEnergyBasePair energy( acc1, racc );

	SECTION("empty output") {
		// output streams
		std::stringstream s1out, s2out;

		// create and directly destroy
		PredictionTrackerProfileMinE * tracker = new PredictionTrackerProfileMinE( energy, &s1out, &s2out, "X");
		// destroy to flush output
		delete tracker; tracker = NULL;

//		LOG(DEBUG)<<"1.s1:\n"<<s1out.str();
//		LOG(DEBUG)<<"1.s2:\n"<<s2out.str();

		// check output
		REQUIRE( boost::regex_match(s1out.str(),boost::regex("^"+r1.getId()+";minE(\\s+\\d;X){"+toString(r1.size())+"}\\s*$"), boost::match_perl) );
		REQUIRE( boost::regex_match(s2out.str(),boost::regex("^"+r2.getId()+";minE(\\s+\\d;X){"+toString(r2.size())+"}\\s*$"), boost::match_perl) );
	}

	SECTION("single range") {
		// output streams
		std::stringstream s1out, s2out;

		// create
		PredictionTrackerProfileMinE * tracker = new PredictionTrackerProfileMinE( energy, &s1out, &s2out, "X");
		// add range
		tracker->updateOptimumCalled( 0,1, 4,5, 2.0 );
		// destroy to flush output
		delete tracker; tracker = NULL;

//		LOG(DEBUG)<<"2.s1:\n"<<s1out.str();
//		LOG(DEBUG)<<"2.s2:\n"<<s2out.str();

		// check output
		REQUIRE( boost::regex_match(s1out.str(),boost::regex("^"+r1.getId()+";minE(\\s+\\d;2){2}(\\s+\\d;X){"+toString(r1.size()-2)+"}\\s*$"), boost::match_perl) );
		REQUIRE( boost::regex_match(s2out.str(),boost::regex("^"+r2.getId()+";minE(\\s+\\d;X){"+toString(r2.size()-2-4)+"}(\\s+\\d;2){2}(\\s+\\d;X){4}\\s*$"), boost::match_perl) );
	}


	SECTION("overwrite update") {
		// output streams
		std::stringstream s1out, s2out;

		// create
		PredictionTrackerProfileMinE * tracker = new PredictionTrackerProfileMinE( energy, &s1out, &s2out, "X");
		// add range
		tracker->updateOptimumCalled( 0,1, 4,5, 2.0 );
		tracker->updateOptimumCalled( 1,2, 5,6, 1.0 );
		// destroy to flush output
		delete tracker; tracker = NULL;

//		LOG(DEBUG)<<"3.s1:\n"<<s1out.str();
//		LOG(DEBUG)<<"3.s2:\n"<<s2out.str();

		// check output
		REQUIRE( boost::regex_match(s1out.str(),boost::regex("^"+r1.getId()+";minE(\\s+\\d;2)(\\s+\\d;1){2}(\\s+\\d;X){"+toString(r1.size()-3)+"}\\s*$"), boost::match_perl) );
		REQUIRE( boost::regex_match(s2out.str(),boost::regex("^"+r2.getId()+";minE(\\s+\\d;X){"+toString(r2.size()-3-4)+"}(\\s+\\d;1){2}(\\s+\\d;2)(\\s+\\d;X){4}\\s*$"), boost::match_perl) );
	}


}
