bool OSExchangeDataProviderAuraX11::HasString() const {
  std::vector< ::Atom> text_atoms = ui::GetTextAtomsFrom(&atom_cache_);
  std::vector< ::Atom> requested_types;
  ui::GetAtomIntersection(text_atoms, GetTargets(), &requested_types);
  return !requested_types.empty() && !HasFile();
}
