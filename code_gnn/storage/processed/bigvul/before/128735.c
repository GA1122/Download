bool TryMatchSearchParam(base::StringPiece text,
                         base::StringPiece pattern,
                         std::string* prefix,
                         std::string* suffix) {
  auto pos = text.find(pattern);
  if (pos == base::StringPiece::npos)
    return false;
  text.substr(0, pos).CopyToString(prefix);
  text.substr(pos + pattern.length()).CopyToString(suffix);
  return true;
}
