HttpUtil::HeadersIterator::HeadersIterator(string::const_iterator headers_begin,
                                           string::const_iterator headers_end,
                                           const std::string& line_delimiter)
    : lines_(headers_begin, headers_end, line_delimiter) {
}
