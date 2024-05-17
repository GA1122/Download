bool HttpUtil::HasHeader(const std::string& headers, const char* name) {
  size_t name_len = strlen(name);
  string::const_iterator it =
      std::search(headers.begin(),
                  headers.end(),
                  name,
                  name + name_len,
                  base::CaseInsensitiveCompareASCII<char>());
  if (it == headers.end())
    return false;

  if (it != headers.begin() && it[-1] != '\n')
    return false;

  if (it + name_len >= headers.end() || it[name_len] != ':')
    return false;

  return true;
}
