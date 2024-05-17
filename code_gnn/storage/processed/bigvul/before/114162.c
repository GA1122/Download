void trim_quote(std::wstring* string) {
  std::wstring::size_type pos1 = string->find_first_not_of(L'"');
  std::wstring::size_type pos2 = string->find_last_not_of(L'"');

  if (std::wstring::npos == pos1 || std::wstring::npos == pos2)
    (*string) = L"";
  else
    (*string) = string->substr(pos1, pos2 + 1);
}
