GURL Extension::GetIconURL(int size,
                           ExtensionIconSet::MatchType match_type) const {
  std::string path = icons().Get(size, match_type);
  return path.empty() ? GURL() : GetResourceURL(path);
}
