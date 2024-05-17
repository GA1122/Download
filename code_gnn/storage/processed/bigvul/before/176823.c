std::vector<std::string> splitAndLowercase(const StringPiece& str, char sep) {
 return splitAndTransform(str, sep, ::tolower);
}
