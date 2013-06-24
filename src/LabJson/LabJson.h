
// Copyright (c) 2013 Nick Porcino, All rights reserved.
// License is MIT: http://opensource.org/licenses/MIT

#pragma once


namespace Lab {
	namespace Json {
			
        class Callback
        {
        public:
            virtual void startArray() = 0;
            virtual void endArray() = 0;
            virtual void startObject() = 0;
            virtual void nameValue(char const*const, int len) = 0;
            virtual void endValue() = 0;
            virtual void endObject() = 0;
            virtual void nullVal() = 0;
            virtual void boolVal(bool) = 0;
            virtual void strVal(char const*const, int len) = 0;
            virtual void floatVal(float) = 0;
            virtual void raiseError(char const*const, int len) = 0;
        };
        
        // return true for success
		bool parseJsonValue(const char*& curr, const char* end, Callback*);
		
	}} // Lab::Json

void testVariables();

