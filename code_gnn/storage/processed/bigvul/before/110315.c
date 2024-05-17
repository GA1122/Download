  bool IsPrintScalingDisabled() {
    if (ppp_printing_ != NULL) {
      PP_Bool result = ppp_printing_->IsScalingDisabled(plugin_->pp_instance());
      return result == PP_TRUE;
    }
    return false;
  }
