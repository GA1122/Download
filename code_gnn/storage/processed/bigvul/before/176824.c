static std::vector<std::string> splitAndTransform(const StringPiece& str, char sep,
 const std::function<char(char)>& f) {
    std::vector<std::string> parts;
 const StringPiece::const_iterator end = std::end(str);
 StringPiece::const_iterator start = std::begin(str);
 StringPiece::const_iterator current;
 do {
        current = std::find(start, end, sep);
        parts.emplace_back(str.substr(start, current).toString());
 if (f) {
            std::string& part = parts.back();
            std::transform(part.begin(), part.end(), part.begin(), f);
 }
        start = current + 1;
 } while (current != end);
 return parts;
}
