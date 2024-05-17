void Browser::UpdateUIForNavigationInTab(TabContents* contents,
                                         content::PageTransition transition,
                                         bool user_initiated) {
  tab_strip_model_->TabNavigating(contents, transition);

  bool contents_is_selected = contents == chrome::GetActiveTabContents(this);
  if (user_initiated && contents_is_selected && window()->GetLocationBar()) {
    window()->GetLocationBar()->Revert();
  }

  if (GetStatusBubble())
    GetStatusBubble()->Hide();

  ScheduleUIUpdate(contents->web_contents(), content::INVALIDATE_TYPE_URL);

  if (contents_is_selected)
    contents->web_contents()->Focus();
}
