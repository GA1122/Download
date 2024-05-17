bool ContextualSearchFieldTrial::IsSendBasePageURLDisabled() {
  return GetBooleanParam(kContextualSearchSendURLDisabledParamName,
                         &is_send_base_page_url_disabled_cached_,
                         &is_send_base_page_url_disabled_);
}
