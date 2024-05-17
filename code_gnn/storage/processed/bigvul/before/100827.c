size_t HttpUtil::FindDelimiter(const string& line, size_t search_start,
                               char delimiter) {
  do {
    const char delim_str[] = { delimiter, '"', '\'', '\0' };
    size_t cur_delim_pos = line.find_first_of(delim_str, search_start);
    if (cur_delim_pos == string::npos)
      return line.length();

    char ch = line[cur_delim_pos];
    if (ch == delimiter) {
      return cur_delim_pos;
    }

    search_start = FindStringEnd(line, cur_delim_pos, ch);
    if (search_start == line.length())
      return search_start;

    ++search_start;

  } while (true);

  NOTREACHED();
  return line.length();
}
