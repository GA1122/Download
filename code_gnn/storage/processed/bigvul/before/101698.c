void Browser::LoadingStateChanged(TabContents* source) {
  window_->UpdateLoadingAnimations(
      tab_handler_->GetTabStripModel()->TabsAreLoading());
  window_->UpdateTitleBar();

  TabContents* selected_contents = GetSelectedTabContents();
  if (source == selected_contents) {
    bool is_loading = source->IsLoading();
    UpdateReloadStopState(is_loading, false);
    if (GetStatusBubble()) {
      GetStatusBubble()->SetStatus(
          GetSelectedTabContentsWrapper()->GetStatusText());
    }

    if (!is_loading && pending_web_app_action_ == UPDATE_SHORTCUT) {
      NavigationEntry* entry = source->controller().GetLastCommittedEntry();
      if (entry) {
        TabContentsWrapper::GetCurrentWrapperForContents(source)->
            extension_tab_helper()->GetApplicationInfo(entry->page_id());
      } else {
        pending_web_app_action_ = NONE;
      }
    }
  }
}
