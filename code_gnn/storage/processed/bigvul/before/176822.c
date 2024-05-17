std::vector<std::string> split(const StringPiece& str, char sep) {
 return splitAndTransform(str, sep, nullptr);
}
