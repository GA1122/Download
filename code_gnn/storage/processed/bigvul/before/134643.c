bool OSExchangeDataProviderAuraX11::HasFile() const {
  std::vector< ::Atom> url_atoms = ui::GetURIListAtomsFrom(&atom_cache_);
  std::vector< ::Atom> requested_types;
  ui::GetAtomIntersection(url_atoms, GetTargets(), &requested_types);

  if (requested_types.empty())
    return false;

  ui::SelectionData data(format_map_.GetFirstOf(requested_types));
  if (data.IsValid()) {
    std::vector<std::string> tokens = ui::ParseURIList(data);
    for (std::vector<std::string>::const_iterator it = tokens.begin();
         it != tokens.end(); ++it) {
      GURL url(*it);
      base::FilePath file_path;
      if (url.SchemeIsFile() && net::FileURLToFilePath(url, &file_path))
        return true;
    }
  }

  return false;
}
