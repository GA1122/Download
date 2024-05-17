void NavigationControllerImpl::LoadURLWithParams(const LoadURLParams& params) {
  TRACE_EVENT0("browser", "NavigationControllerImpl::LoadURLWithParams");
  if (HandleDebugURL(params.url, params.transition_type))
    return;

  switch (params.load_type) {
    case LOAD_TYPE_DEFAULT:
      break;
    case LOAD_TYPE_BROWSER_INITIATED_HTTP_POST:
      if (!params.url.SchemeIs(chrome::kHttpScheme) &&
          !params.url.SchemeIs(kHttpsScheme)) {
        NOTREACHED() << "Http post load must use http(s) scheme.";
        return;
      }
      break;
    case LOAD_TYPE_DATA:
      if (!params.url.SchemeIs(chrome::kDataScheme)) {
        NOTREACHED() << "Data load must use data scheme.";
        return;
      }
      break;
    default:
      NOTREACHED();
      break;
  };

  needs_reload_ = false;

  bool override = false;
  switch (params.override_user_agent) {
    case UA_OVERRIDE_INHERIT:
      override = ShouldKeepOverride(GetLastCommittedEntry());
      break;
    case UA_OVERRIDE_TRUE:
      override = true;
      break;
    case UA_OVERRIDE_FALSE:
      override = false;
      break;
    default:
      NOTREACHED();
      break;
  }

  NavigationEntryImpl* entry = NavigationEntryImpl::FromNavigationEntry(
      CreateNavigationEntry(
          params.url,
          params.referrer,
          params.transition_type,
          params.is_renderer_initiated,
          params.extra_headers,
          browser_context_));
  if (params.should_replace_current_entry)
    entry->set_should_replace_entry(true);
  entry->set_should_clear_history_list(params.should_clear_history_list);
  entry->SetIsOverridingUserAgent(override);
  entry->set_transferred_global_request_id(
      params.transferred_global_request_id);
  entry->SetFrameToNavigate(params.frame_name);

  switch (params.load_type) {
    case LOAD_TYPE_DEFAULT:
      break;
    case LOAD_TYPE_BROWSER_INITIATED_HTTP_POST:
      entry->SetHasPostData(true);
      entry->SetBrowserInitiatedPostData(
          params.browser_initiated_post_data.get());
      break;
    case LOAD_TYPE_DATA:
      entry->SetBaseURLForDataURL(params.base_url_for_data_url);
      entry->SetVirtualURL(params.virtual_url_for_data_url);
      entry->SetCanLoadLocalResources(params.can_load_local_resources);
      break;
    default:
      NOTREACHED();
      break;
  };

  LoadEntry(entry);
}
