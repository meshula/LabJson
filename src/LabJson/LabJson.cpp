
// Copyright (c) 2013 Nick Porcino, All rights reserved.
// License is MIT: http://opensource.org/licenses/MIT

#include "LabJson.h"
#include "LabText/ftoa.h"
#include "LabText/itoa.h"
#include "LabText/TextScanner.h"

#include <string>

namespace Lab {
	namespace Json {
		
		std::string jsonEscapedString(const std::string& s)
		{
			std::string ret;
			ret.reserve(s.size());
			for (std::string::const_iterator i = s.begin(); i != s.end(); ++i) {
				char c = *i;
				switch(c) {
					case '"': ret += "\\\""; break;
					case '\\': ret += "\\\\"; break;
					case '/': ret += "\\/"; break;
					case 8: ret += "\\b"; break;
					case 9: ret += "\\t"; break;
					case 10: ret += "\\n"; break; 
					case 12: ret += "\\f"; break;
					case 13: ret += "\\r"; break;
					default: ret += c; break;
				}
			}
			return ret;
		}
		
		
		bool parseJsonObject(const char*& curr, const char* end, Callback* callback)
		{
			curr = tsSkipCommentsAndWhitespace(curr, end);
			if (*curr != '{') {
                callback->raiseError(curr, 32);
				return false;
			}
			++curr;

            bool ret = true;
            callback->startObject();
            
			while (curr < end) {
				curr = tsSkipCommentsAndWhitespace(curr, end);
                char delim = *curr; // " or '
                if (!(delim == '\'' || delim == '\"')) {
                    callback->raiseError(curr, 32);
                    ret = false;
                    break;
				}
				++curr;
				
				// find a string, followed by :, remove any spaces between name and colon
				const char* name;
				uint32_t nameLen;
				curr = tsSkipCommentsAndWhitespace(curr, end);
                curr = tsGetToken(curr, end, delim, &name, &nameLen);
				if (curr == end) {
                    callback->raiseError(curr, 32);
                    ret = false;
                    break;
				}
                callback->nameValue(name, nameLen);
				++curr;
				
				curr = tsSkipCommentsAndWhitespace(curr, end);
				if (*curr != ':') {
                    callback->raiseError(curr, 32);
                    ret = false;
                    break;
				}
				++curr;

				parseJsonValue(curr, end, callback);
				curr = tsSkipCommentsAndWhitespace(curr, end);
				
                callback->endValue();
                
				if (*curr == '}') {
					++curr;
					break;
				}
				
				if (*curr == ',') {
					++curr;
					continue;
				}
				
                callback->raiseError(curr, 32);
                ret = false;
                break;
			}
			
			callback->endObject();
            return ret;
		}
		
		bool parseJsonArray(const char*& curr, const char* end, Callback* callback)
		{
			if (*curr != '[') {
                callback->raiseError(curr, 32);
                return false;
			}
			
            callback->startArray();
            
			++curr;
			while (*curr != ']') {
                if (!parseJsonValue(curr, end, callback)) {
                    break;
                }
				
				curr = tsSkipCommentsAndWhitespace(curr, end);
				if (*curr == ',') {
					++curr;
					continue;
				}
			} // while
            
            ++curr; // skip closing bracket
            
            callback->endArray();
            return true;
		}
		
		bool parseJsonValue(const char*& curr, const char* end, Callback* callback)
		{
            bool ret = true;
            
			curr = tsSkipCommentsAndWhitespace(curr, end);
			switch (*curr) {
				case '{':
					// parse a json object and consume closing brace
					ret = parseJsonObject(curr, end, callback);
                    break;
					
				case '[':
					ret = parseJsonArray(curr, end, callback);
                    break;
					
				case 't': {
						char const* newCurr = tsExpect(curr, end, "true");
                        if (newCurr != curr) {
                            curr = newCurr;
                            callback->boolVal(true);
                        }
                        else {
                            ret = false;
                            callback->raiseError(curr, 32);
                        }
					}
                    break;
					
				case 'f': {
						char const* newCurr = tsExpect(curr, end, "false");
                        if (newCurr != curr) {
                            curr = newCurr;
                            callback->boolVal(false);
                        }
                        else {
                            ret = false;
                            callback->raiseError(curr, 32);
                        }
					}
                    break;
					
				case 'n': {
						char const* newCurr = tsExpect(curr, end, "null");
                        if (newCurr != curr) {
                            curr = newCurr;
                            callback->nullVal();
                        }
                        else {
                            ret = false;
                            callback->raiseError(curr, 32);
                        }
					}
                    break;
					
				case '\"': {
						char const* str;
						uint32_t len;
						curr = tsGetString2(curr, end, '\"', true, &str, &len);
                        callback->strVal(str, len);
					}
                    break;
					
				case '\'': {
                        char const* str;
                        uint32_t len;
                        curr = tsGetString2(curr, end, '\'', true, &str, &len);
                        callback->strVal(str, len);
                    }
                    break;
					
				case '-':
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9': {
						float result = 0;
						curr = tsGetFloat(curr, end, &result);
                        callback->floatVal(result);
					}
                    break;
			}
            return ret;
		} // parseJsonValue
		
	} // Json
}	// Lab


