void Browser::TabInsertedAt(TabContentsWrapper* contents,
                            int index,
                            bool foreground) {
  SetAsDelegate(contents, this);
  contents->restore_tab_helper()->SetWindowID(session_id());

  SyncHistoryWithTabs(index);

  LoadingStateChanged(contents->tab_contents());

  registrar_.Add(this, content::NOTIFICATION_TAB_CONTENTS_DISCONNECTED,
                 Source<TabContents>(contents->tab_contents()));

  registrar_.Add(this, content::NOTIFICATION_INTERSTITIAL_ATTACHED,
                 Source<TabContents>(contents->tab_contents()));
}
