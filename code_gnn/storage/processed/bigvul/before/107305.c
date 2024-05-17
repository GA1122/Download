void Browser::LoadingStateChanged(TabContents* source) {
  window_->UpdateLoadingAnimations(
      tab_handler_->GetTabStripModel()->TabsAreLoading());
  window_->UpdateTitleBar();

  TabContents* selected_contents = GetSelectedTabContents();
  if (source == selected_contents) {
    UpdateReloadStopState(source->is_loading(), false);
    if (GetStatusBubble()) {
      GetStatusBubble()->SetStatus(GetSelectedTabContents()->GetStatusText());
    }

    if (!source->is_loading() &&
        pending_web_app_action_ == UPDATE_SHORTCUT) {
      NavigationEntry* entry = source->controller().GetLastCommittedEntry();
      if (entry) {
        source->render_view_host()->GetApplicationInfo(entry->page_id());
      } else {
        pending_web_app_action_ = NONE;
      }
    }
  }
}
