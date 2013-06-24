
// Copyright (c) 2013 Nick Porcino, All rights reserved.
// License is MIT: http://opensource.org/licenses/MIT

#pragma once

#include "LabJson.h"

// Implements a subset of http://goessner.net/articles/JsonPath/
// See the testPath function for examples that work

// Not implemented: [,] union operator, eg [1, 3, 4]
// Not implemented: predicator, eg $..book[?(@.isbn)] filters books with an isbn field
// Not impelementd: expression filter, $..book[?(@.price<10)] books costing less than 10
// Not implemented: expression: $..book[(@.length-1)] last book

namespace Lab {
	namespace Json {

#if 0
    Todo: This matches the Json implementation on github, needs to be updated
		ValuePtr jsonPath(const char*& expr, ValuePtr o);
		
		void testPath();
#endif
		
	}
}


