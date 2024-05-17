bool verifyJavaStringFormat(const StringPiece16& str) {
 const char16_t* c = str.begin();
 const char16_t* const end = str.end();

 size_t argCount = 0;
 bool nonpositional = false;
 while (c != end) {
 if (*c == u'%' && c + 1 < end) {
            c++;

 if (*c == u'%') {
                c++;
 continue;
 }

            argCount++;

 size_t numDigits = consumeDigits(c, end);
 if (numDigits > 0) {
                c += numDigits;
 if (c != end && *c != u'$') {
                    nonpositional = true;
 }
 } else if (*c == u'<') {
                nonpositional = true;

                c++;

 if (c != end && *c == u'$') {
                    c++;
 }
 } else {
                nonpositional = true;
 }

 while (c != end && (*c == u'-' ||
 *c == u'#' ||
 *c == u'+' ||
 *c == u' ' ||
 *c == u',' ||
 *c == u'(' ||
 (*c >= u'0' && *c <= '9'))) {
                c++;
 }

  
 if (c != end) {
 switch (*c) {
 case 'D':
 case 'F':
 case 'K':
 case 'M':
 case 'W':
 case 'Z':
 case 'k':
 case 'm':
 case 'w':
 case 'y':
 case 'z':
 return true;
 }
 }
 }

 if (c != end) {
            c++;
 }
 }

 if (argCount > 1 && nonpositional) {
 return false;
 }
 return true;
}
