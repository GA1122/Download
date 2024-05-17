OSExchangeDataProviderAuraX11::OSExchangeDataProviderAuraX11()
    : x_display_(gfx::GetXDisplay()),
      x_root_window_(DefaultRootWindow(x_display_)),
      own_window_(true),
      x_window_(XCreateWindow(
          x_display_,
          x_root_window_,
          -100, -100, 10, 10,   
          0,                    
          CopyFromParent,       
          InputOnly,
          CopyFromParent,       
          0,
          NULL)),
      atom_cache_(x_display_, kAtomsToCache),
      format_map_(),
      selection_owner_(x_display_, x_window_,
                       atom_cache_.GetAtom(kDndSelection)) {
  atom_cache_.allow_uncached_atoms();

  XStoreName(x_display_, x_window_, "Chromium Drag & Drop Window");

  base::MessagePumpX11::Current()->AddDispatcherForWindow(this, x_window_);
}
