void VerifySyncedSession(
    const std::string& tag,
    const std::vector<std::vector<SessionID::id_type> >& windows,
    const SyncedSession& session) {
  ASSERT_EQ(tag, session.session_tag);
  ASSERT_EQ(SyncedSession::TYPE_LINUX, session.device_type);
  ASSERT_EQ("name", session.session_name);
  ASSERT_EQ(windows.size(), session.windows.size());

  int i = 0;
  for (std::vector<std::vector<int> >::const_iterator win_iter =
           windows.begin();
       win_iter != windows.end(); ++win_iter, ++i) {
    SessionWindow* win_ptr;
    SyncedSession::SyncedWindowMap::const_iterator map_iter =
        session.windows.find(i);
    if (map_iter != session.windows.end())
      win_ptr = map_iter->second;
    else
      FAIL();
    ASSERT_EQ(win_iter->size(), win_ptr->tabs.size());
    ASSERT_EQ(0, win_ptr->selected_tab_index);
    ASSERT_EQ(1, win_ptr->type);
    int j = 0;
    for (std::vector<int>::const_iterator tab_iter = (*win_iter).begin();
         tab_iter != (*win_iter).end(); ++tab_iter, ++j) {
      SessionTab* tab = win_ptr->tabs[j];
      ASSERT_EQ(*tab_iter, tab->tab_id.id());
      ASSERT_EQ(1U, tab->navigations.size());
      ASSERT_EQ(1, tab->tab_visual_index);
      ASSERT_EQ(0, tab->current_navigation_index);
      ASSERT_TRUE(tab->pinned);
      ASSERT_EQ("app_id", tab->extension_app_id);
      ASSERT_EQ(1U, tab->navigations.size());
      ASSERT_EQ(12, tab->navigations[0].index());
      ASSERT_EQ(tab->navigations[0].virtual_url(), GURL("http://foo/1"));
      ASSERT_EQ(tab->navigations[0].referrer().url, GURL("referrer"));
      ASSERT_EQ(tab->navigations[0].title(), string16(ASCIIToUTF16("title")));
      ASSERT_EQ(tab->navigations[0].transition(),
                content::PAGE_TRANSITION_TYPED);
    }
  }
}
