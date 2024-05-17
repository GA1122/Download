  bool StartFind(const std::string& text, bool case_sensitive) {
    if (ppp_find_ != NULL) {
      PP_Bool pp_success =
          ppp_find_->StartFind(plugin_->pp_instance(),
                               text.c_str(),
                               PP_FromBool(case_sensitive));
      return pp_success == PP_TRUE;
    }
    return false;
  }
