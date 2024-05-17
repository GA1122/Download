void Browser::LoadingStateChanged(WebContents* source) {
  window_->UpdateLoadingAnimations(tab_strip_model_->TabsAreLoading());
  window_->UpdateTitleBar();

  WebContents* selected_contents = chrome::GetActiveWebContents(this);
  if (source == selected_contents) {
    bool is_loading = source->IsLoading();
    command_controller_->LoadingStateChanged(is_loading, false);
    if (GetStatusBubble()) {
      GetStatusBubble()->SetStatus(
          CoreTabHelper::FromWebContents(chrome::GetActiveWebContents(this))->
              GetStatusText());
    }
  }
}
