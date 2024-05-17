void Browser::UpdateUIForNavigationInTab(TabContentsWrapper* contents,
                                         PageTransition::Type transition,
                                         bool user_initiated) {
  tabstrip_model()->TabNavigating(contents, transition);

  bool contents_is_selected = contents == GetSelectedTabContentsWrapper();
  if (user_initiated && contents_is_selected && window()->GetLocationBar()) {
    window()->GetLocationBar()->Revert();
  }

  if (GetStatusBubble())
    GetStatusBubble()->Hide();

  ScheduleUIUpdate(contents->tab_contents(), TabContents::INVALIDATE_URL);

  if (contents_is_selected)
    contents->tab_contents()->Focus();
}
