void WebKitTestController::OnCaptureSessionHistory() {
  std::vector<int> routing_ids;
  std::vector<std::vector<PageState> > session_histories;
  std::vector<unsigned> current_entry_indexes;

  RenderViewHost* render_view_host =
      main_window_->web_contents()->GetRenderViewHost();

  for (std::vector<Shell*>::iterator window = Shell::windows().begin();
       window != Shell::windows().end();
       ++window) {
    WebContents* web_contents = (*window)->web_contents();
    if (render_view_host->GetProcess() !=
        web_contents->GetRenderViewHost()->GetProcess()) {
      continue;
    }
    routing_ids.push_back(web_contents->GetRenderViewHost()->GetRoutingID());
    current_entry_indexes.push_back(
        web_contents->GetController().GetCurrentEntryIndex());
    std::vector<PageState> history;
    for (int entry = 0; entry < web_contents->GetController().GetEntryCount();
         ++entry) {
      PageState state = web_contents->GetController().GetEntryAtIndex(entry)->
          GetPageState();
      if (!state.IsValid()) {
        state = PageState::CreateFromURL(
            web_contents->GetController().GetEntryAtIndex(entry)->GetURL());
      }
      history.push_back(state);
    }
    session_histories.push_back(history);
  }

  Send(new ShellViewMsg_SessionHistory(render_view_host->GetRoutingID(),
                                       routing_ids,
                                       session_histories,
                                       current_entry_indexes));
}
