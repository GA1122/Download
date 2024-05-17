bool NavigationController::RendererDidNavigate(
    const ViewHostMsg_FrameNavigate_Params& params,
    int extra_invalidate_flags,
    LoadCommittedDetails* details) {
  if (GetLastCommittedEntry()) {
    details->previous_url = GetLastCommittedEntry()->url();
    details->previous_entry_index = last_committed_entry_index();
  } else {
    details->previous_url = GURL();
    details->previous_entry_index = -1;
  }

  if (pending_entry_index_ >= 0)
    pending_entry_->set_site_instance(tab_contents_->GetSiteInstance());

  details->type = ClassifyNavigation(params);
  switch (details->type) {
    case NavigationType::NEW_PAGE:
      RendererDidNavigateToNewPage(params, &(details->did_replace_entry));
      break;
    case NavigationType::EXISTING_PAGE:
      RendererDidNavigateToExistingPage(params);
      break;
    case NavigationType::SAME_PAGE:
      RendererDidNavigateToSamePage(params);
      break;
    case NavigationType::IN_PAGE:
      RendererDidNavigateInPage(params, &(details->did_replace_entry));
      break;
    case NavigationType::NEW_SUBFRAME:
      RendererDidNavigateNewSubframe(params);
      break;
    case NavigationType::AUTO_SUBFRAME:
      if (!RendererDidNavigateAutoSubframe(params))
        return false;
      break;
    case NavigationType::NAV_IGNORE:
      return false;
    default:
      NOTREACHED();
  }

  SetContentStateIfEmpty(GetActiveEntry());

  details->is_auto = (PageTransition::IsRedirect(params.transition) &&
                      !pending_entry()) ||
      params.gesture == NavigationGestureAuto;

  details->entry = GetActiveEntry();
  details->is_in_page = IsURLInPageNavigation(params.url);
  details->is_main_frame = PageTransition::IsMainFrame(params.transition);
  details->serialized_security_info = params.security_info;
  details->is_content_filtered = params.is_content_filtered;
  details->http_status_code = params.http_status_code;
  NotifyNavigationEntryCommitted(details, extra_invalidate_flags);

  user_gesture_observed_ = false;

  return true;
}
