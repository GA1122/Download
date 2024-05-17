bool ShouldTreatNavigationAsReload(
    const GURL& url,
    const GURL& virtual_url,
    const GURL& base_url_for_data_url,
    ui::PageTransition transition_type,
    bool is_main_frame,
    bool is_post,
    bool is_reload,
    bool is_navigation_to_existing_entry,
    bool has_interstitial,
    const NavigationEntryImpl* last_committed_entry) {
  if (has_interstitial)
    return false;

  if (!is_main_frame || is_reload || is_navigation_to_existing_entry)
    return false;

  if (!last_committed_entry)
    return false;

  if (transition_type & ui::PAGE_TRANSITION_FROM_API)
    return false;

  bool transition_type_can_be_converted = false;
  if (ui::PageTransitionCoreTypeIs(transition_type,
                                   ui::PAGE_TRANSITION_RELOAD) &&
      (transition_type & ui::PAGE_TRANSITION_FROM_ADDRESS_BAR)) {
    transition_type_can_be_converted = true;
  }
  if (ui::PageTransitionCoreTypeIs(transition_type,
                                   ui::PAGE_TRANSITION_TYPED)) {
    transition_type_can_be_converted = true;
  }
  if (ui::PageTransitionCoreTypeIs(transition_type, ui::PAGE_TRANSITION_LINK))
    transition_type_can_be_converted = true;
  if (!transition_type_can_be_converted)
    return false;

  if (virtual_url != last_committed_entry->GetVirtualURL())
    return false;

  if (url != last_committed_entry->GetURL())
    return false;

  if (url.SchemeIs(url::kDataScheme) && base_url_for_data_url.is_valid()) {
    if (base_url_for_data_url != last_committed_entry->GetBaseURLForDataURL())
      return false;
  }

  if (last_committed_entry->ssl_error())
    return false;

  if (last_committed_entry->GetHasPostData() || is_post)
    return false;

  return true;
}
