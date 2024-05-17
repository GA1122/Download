StringPiece16 trimWhitespace(const StringPiece16& str) {
 if (str.size() == 0 || str.data() == nullptr) {
 return str;
 }

 const char16_t* start = str.data();
 const char16_t* end = str.data() + str.length();

 while (start != end && util::isspace16(*start)) {
        start++;
 }

 while (end != start && util::isspace16(*(end - 1))) {
        end--;
 }

 return StringPiece16(start, end - start);
}
