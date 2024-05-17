StringPiece trimWhitespace(const StringPiece& str) {
 if (str.size() == 0 || str.data() == nullptr) {
 return str;
 }

 const char* start = str.data();
 const char* end = str.data() + str.length();

 while (start != end && isspace(*start)) {
        start++;
 }

 while (end != start && isspace(*(end - 1))) {
        end--;
 }

 return StringPiece(start, end - start);
}
