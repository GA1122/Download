bool OSExchangeDataProviderAuraX11::GetPickledData(
    const OSExchangeData::CustomFormat& format,
    Pickle* pickle) const {
  std::vector< ::Atom> requested_types;
  requested_types.push_back(atom_cache_.GetAtom(format.ToString().c_str()));

  ui::SelectionData data(format_map_.GetFirstOf(requested_types));
  if (data.IsValid()) {
    *pickle = Pickle(reinterpret_cast<const char*>(data.GetData()),
                     static_cast<int>(data.GetSize()));
    return true;
  }

  return false;
}
