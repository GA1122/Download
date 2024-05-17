void PageInfo::RecordPageInfoAction(PageInfoAction action) {
  if (action != PAGE_INFO_OPENED)
    did_perform_action_ = true;

  UMA_HISTOGRAM_ENUMERATION("WebsiteSettings.Action", action, PAGE_INFO_COUNT);

  std::string histogram_name;
  if (site_url_.SchemeIsCryptographic()) {
    if (security_level_ == security_state::SECURE) {
      UMA_HISTOGRAM_ENUMERATION("Security.PageInfo.Action.HttpsUrl.ValidNonEV",
                                action, PAGE_INFO_COUNT);
    } else if (security_level_ == security_state::EV_SECURE) {
      UMA_HISTOGRAM_ENUMERATION("Security.PageInfo.Action.HttpsUrl.ValidEV",
                                action, PAGE_INFO_COUNT);
    } else if (security_level_ == security_state::NONE) {
      UMA_HISTOGRAM_ENUMERATION("Security.PageInfo.Action.HttpsUrl.Downgraded",
                                action, PAGE_INFO_COUNT);
    } else if (security_level_ == security_state::DANGEROUS) {
      UMA_HISTOGRAM_ENUMERATION("Security.PageInfo.Action.HttpsUrl.Dangerous",
                                action, PAGE_INFO_COUNT);
    }
    return;
  }

  if (security_level_ == security_state::HTTP_SHOW_WARNING) {
    UMA_HISTOGRAM_ENUMERATION("Security.PageInfo.Action.HttpUrl.Warning",
                              action, PAGE_INFO_COUNT);
  } else if (security_level_ == security_state::DANGEROUS) {
    UMA_HISTOGRAM_ENUMERATION("Security.PageInfo.Action.HttpUrl.Dangerous",
                              action, PAGE_INFO_COUNT);
  } else {
    UMA_HISTOGRAM_ENUMERATION("Security.PageInfo.Action.HttpUrl.Neutral",
                              action, PAGE_INFO_COUNT);
  }
}
