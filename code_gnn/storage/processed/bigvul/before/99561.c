static void DecodeWebSafe(std::string* decoded) {
  DCHECK(decoded);
  for (std::string::iterator i(decoded->begin()); i != decoded->end(); ++i) {
    if (*i == '_')
      *i = '/';
    else if (*i == '-')
      *i = '+';
  }
}
