bool NavigationControllerImpl::RendererDidNavigate(
    const ViewHostMsg_FrameNavigate_Params& params,
    LoadCommittedDetails* details) {
  is_initial_navigation_ = false;

  if (GetLastCommittedEntry()) {
    details->previous_url = GetLastCommittedEntry()->GetURL();
    details->previous_entry_index = GetLastCommittedEntryIndex();
  } else {
    details->previous_url = GURL();
    details->previous_entry_index = -1;
  }

  DCHECK(pending_entry_index_ == -1 || pending_entry_->site_instance());

  details->did_replace_entry =
      pending_entry_ && pending_entry_->should_replace_entry();

  details->type = ClassifyNavigation(params);

  details->is_in_page = IsURLInPageNavigation(
      params.url, params.was_within_same_page, details->type);

  switch (details->type) {
    case NAVIGATION_TYPE_NEW_PAGE:
      RendererDidNavigateToNewPage(params, details->did_replace_entry);
      break;
    case NAVIGATION_TYPE_EXISTING_PAGE:
      RendererDidNavigateToExistingPage(params);
      break;
    case NAVIGATION_TYPE_SAME_PAGE:
      RendererDidNavigateToSamePage(params);
      break;
    case NAVIGATION_TYPE_IN_PAGE:
      RendererDidNavigateInPage(params, &details->did_replace_entry);
      break;
    case NAVIGATION_TYPE_NEW_SUBFRAME:
      RendererDidNavigateNewSubframe(params);
      break;
    case NAVIGATION_TYPE_AUTO_SUBFRAME:
      if (!RendererDidNavigateAutoSubframe(params))
        return false;
      break;
    case NAVIGATION_TYPE_NAV_IGNORE:
      if (pending_entry_) {
        DiscardNonCommittedEntries();
        web_contents_->NotifyNavigationStateChanged(INVALIDATE_TYPE_URL);
      }
      return false;
    default:
      NOTREACHED();
  }

  base::Time timestamp =
      time_smoother_.GetSmoothedTime(get_timestamp_callback_.Run());
  DVLOG(1) << "Navigation finished at (smoothed) timestamp "
           << timestamp.ToInternalValue();

  DiscardNonCommittedEntriesInternal();

  DCHECK(params.page_state.IsValid());
  NavigationEntryImpl* active_entry =
      NavigationEntryImpl::FromNavigationEntry(GetLastCommittedEntry());
  active_entry->SetTimestamp(timestamp);
  active_entry->SetHttpStatusCode(params.http_status_code);
  active_entry->SetPageState(params.page_state);
  active_entry->SetBrowserInitiatedPostData(NULL);

  active_entry->set_is_renderer_initiated(false);

  active_entry->set_should_clear_history_list(false);

  CHECK(active_entry->site_instance() == web_contents_->GetSiteInstance());

  active_entry->SetBindings(
      web_contents_->GetRenderViewHost()->GetEnabledBindings());

  details->entry = active_entry;
  details->is_main_frame =
      PageTransitionIsMainFrame(params.transition);
  details->serialized_security_info = params.security_info;
  details->http_status_code = params.http_status_code;
  NotifyNavigationEntryCommitted(details);

  return true;
}
