void Browser::ContentsMouseEvent(WebContents* source,
                                 bool motion,
                                 bool exited) {
  exclusive_access_manager_->OnUserInput();

  if (!GetStatusBubble() || (!motion && !exited))
    return;

  if (source == tab_strip_model_->GetActiveWebContents()) {
    GetStatusBubble()->MouseMoved(exited);
    if (exited)
      GetStatusBubble()->SetURL(GURL());
  }
}
