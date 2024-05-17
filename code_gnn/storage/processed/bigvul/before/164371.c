  bool populate_tabs() {
    if (params_->get_info.get() && params_->get_info->populate.get())
      return *params_->get_info->populate;
    return false;
  }
