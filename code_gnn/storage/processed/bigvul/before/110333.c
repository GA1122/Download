  uint32_t QuerySupportedPrintOutputFormats() {
    if (ppp_printing_ != NULL) {
      return ppp_printing_->QuerySupportedFormats(plugin_->pp_instance());
    }
    return 0;
  }
