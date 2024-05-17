content::NavigationType NavigationController::ClassifyNavigation(
    const ViewHostMsg_FrameNavigate_Params& params) const {
  if (params.page_id == -1) {
    return content::NAVIGATION_TYPE_NAV_IGNORE;
  }

  if (params.page_id > tab_contents_->GetMaxPageID()) {
    if (content::PageTransitionIsMainFrame(params.transition))
      return content::NAVIGATION_TYPE_NEW_PAGE;

    if (!GetLastCommittedEntry())
      return content::NAVIGATION_TYPE_NAV_IGNORE;

    return content::NAVIGATION_TYPE_NEW_SUBFRAME;
  }

  int existing_entry_index = GetEntryIndexWithPageID(
      tab_contents_->GetSiteInstance(),
      params.page_id);
  if (existing_entry_index == -1) {
    NOTREACHED();

    LOG(ERROR) << "terminating renderer for bad navigation: " << params.url;
    content::RecordAction(UserMetricsAction("BadMessageTerminate_NC"));

    std::string temp = params.url.spec();
    temp.append("#page");
    temp.append(base::IntToString(params.page_id));
    temp.append("#max");
    temp.append(base::IntToString(tab_contents_->GetMaxPageID()));
    temp.append("#frame");
    temp.append(base::IntToString(params.frame_id));
    temp.append("#ids");
    for (int i = 0; i < static_cast<int>(entries_.size()); ++i) {
      temp.append(base::IntToString(entries_[i]->page_id()));
      temp.append("_");
      if (entries_[i]->site_instance())
        temp.append(base::IntToString(entries_[i]->site_instance()->id()));
      else
        temp.append("N");
      if (entries_[i]->site_instance() != tab_contents_->GetSiteInstance())
        temp.append("x");
      temp.append(",");
    }
    GURL url(temp);
    tab_contents_->render_view_host()->Send(new ViewMsg_TempCrashWithData(url));
    return content::NAVIGATION_TYPE_NAV_IGNORE;


    if (tab_contents_->GetSiteInstance()->HasProcess())
      tab_contents_->GetSiteInstance()->GetProcess()->ReceivedBadMessage();
    return content::NAVIGATION_TYPE_NAV_IGNORE;
  }
  NavigationEntry* existing_entry = entries_[existing_entry_index].get();

  if (!content::PageTransitionIsMainFrame(params.transition)) {
    DCHECK(GetLastCommittedEntry());
    return content::NAVIGATION_TYPE_AUTO_SUBFRAME;
  }

  if (pending_entry_ &&
      existing_entry != pending_entry_ &&
      pending_entry_->page_id() == -1 &&
      existing_entry == GetLastCommittedEntry()) {
    return content::NAVIGATION_TYPE_SAME_PAGE;
  }

  if (AreURLsInPageNavigation(existing_entry->url(), params.url))
    return content::NAVIGATION_TYPE_IN_PAGE;

  return content::NAVIGATION_TYPE_EXISTING_PAGE;
}
