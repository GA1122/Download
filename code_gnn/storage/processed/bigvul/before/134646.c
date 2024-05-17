bool OSExchangeDataProviderAuraX11::HasURL() const {
  std::vector< ::Atom> url_atoms = ui::GetURLAtomsFrom(&atom_cache_);
  std::vector< ::Atom> requested_types;
  ui::GetAtomIntersection(url_atoms, GetTargets(), &requested_types);

  if (requested_types.empty())
    return false;

  ui::SelectionData data(format_map_.GetFirstOf(requested_types));
  if (data.IsValid()) {
    if (data.GetType() == atom_cache_.GetAtom(kMimeTypeMozillaURL)) {
      return true;
    } else if (data.GetType() == atom_cache_.GetAtom(
        ui::Clipboard::kMimeTypeURIList)) {
      std::vector<std::string> tokens = ui::ParseURIList(data);
      for (std::vector<std::string>::const_iterator it = tokens.begin();
           it != tokens.end(); ++it) {
        if (!GURL(*it).SchemeIsFile())
          return true;
      }

      return false;
    }
  }

  return false;
}
