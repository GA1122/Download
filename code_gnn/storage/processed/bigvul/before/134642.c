bool OSExchangeDataProviderAuraX11::HasCustomFormat(
    const OSExchangeData::CustomFormat& format) const {
  std::vector< ::Atom> url_atoms;
  url_atoms.push_back(atom_cache_.GetAtom(format.ToString().c_str()));
  std::vector< ::Atom> requested_types;
  ui::GetAtomIntersection(url_atoms, GetTargets(), &requested_types);

  return !requested_types.empty();
}
