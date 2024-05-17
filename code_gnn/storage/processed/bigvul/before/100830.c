static size_t FindStringEnd(const string& line, size_t start, char delim) {
  DCHECK(start < line.length() && line[start] == delim &&
         (delim == '"' || delim == '\''));

  const char set[] = { delim, '\\', '\0' };
  for (;;) {

    size_t end = line.find_first_of(set, start + 1);
    if (end == string::npos)
      return line.length();

    if (line[end] == '\\') {
      start = end + 1;
      if (start == line.length())
        return start;

      continue;
    }

    return end;
  }

  NOTREACHED();
  return line.length();
}
