void Browser::TabInsertedAt(TabContentsWrapper* contents,
                            int index,
                            bool foreground) {
  SetAsDelegate(contents, this);
  contents->controller().SetWindowID(session_id());

  SyncHistoryWithTabs(index);

  LoadingStateChanged(contents->tab_contents());

  registrar_.Add(this, NotificationType::TAB_CONTENTS_DISCONNECTED,
                 Source<TabContentsWrapper>(contents));
}
