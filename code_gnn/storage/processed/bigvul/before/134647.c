OSExchangeDataProviderAuraX11::OSExchangeDataProviderAuraX11(
    ::Window x_window,
    const SelectionFormatMap& selection)
    : x_display_(gfx::GetXDisplay()),
      x_root_window_(DefaultRootWindow(x_display_)),
      own_window_(false),
      x_window_(x_window),
      atom_cache_(x_display_, kAtomsToCache),
      format_map_(selection),
      selection_owner_(x_display_, x_window_,
                       atom_cache_.GetAtom(kDndSelection)) {
  atom_cache_.allow_uncached_atoms();
}
