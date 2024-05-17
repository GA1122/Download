bool NavigationController::RendererDidNavigate(
    const ViewHostMsg_FrameNavigate_Params& params,
    content::LoadCommittedDetails* details) {

  if (GetLastCommittedEntry()) {
    details->previous_url = GetLastCommittedEntry()->url();
    details->previous_entry_index = last_committed_entry_index();
  } else {
    details->previous_url = GURL();
    details->previous_entry_index = -1;
  }

  DCHECK(pending_entry_index_ == -1 || pending_entry_->site_instance());

  details->is_in_page = IsURLInPageNavigation(params.url);

  details->type = ClassifyNavigation(params);

  switch (details->type) {
    case content::NAVIGATION_TYPE_NEW_PAGE:
      RendererDidNavigateToNewPage(params, &(details->did_replace_entry));
      break;
    case content::NAVIGATION_TYPE_EXISTING_PAGE:
      RendererDidNavigateToExistingPage(params);
      break;
    case content::NAVIGATION_TYPE_SAME_PAGE:
      RendererDidNavigateToSamePage(params);
      break;
    case content::NAVIGATION_TYPE_IN_PAGE:
      RendererDidNavigateInPage(params, &(details->did_replace_entry));
      break;
    case content::NAVIGATION_TYPE_NEW_SUBFRAME:
      RendererDidNavigateNewSubframe(params);
      break;
    case content::NAVIGATION_TYPE_AUTO_SUBFRAME:
      if (!RendererDidNavigateAutoSubframe(params))
        return false;
      break;
    case content::NAVIGATION_TYPE_NAV_IGNORE:
      if (pending_entry_) {
        DiscardNonCommittedEntries();
        tab_contents_->NotifyNavigationStateChanged(
            TabContents::INVALIDATE_URL);
      }
      return false;
    default:
      NOTREACHED();
  }

  DCHECK(!params.content_state.empty());
  NavigationEntry* active_entry = GetActiveEntry();
  active_entry->set_content_state(params.content_state);

  active_entry->set_is_renderer_initiated(false);

  DCHECK(active_entry->site_instance() == tab_contents_->GetSiteInstance());

  details->entry = active_entry;
  details->is_main_frame =
      content::PageTransitionIsMainFrame(params.transition);
  details->serialized_security_info = params.security_info;
  details->http_status_code = params.http_status_code;
  NotifyNavigationEntryCommitted(details);

  return true;
}
