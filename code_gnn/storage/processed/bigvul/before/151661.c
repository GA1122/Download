void Browser::ContentsMouseEvent(WebContents* source,
                                 const gfx::Point& location,
                                 bool motion,
                                 bool exited) {
  exclusive_access_manager_->OnUserInput();

  if (!GetStatusBubble() || (!motion && !exited))
    return;

  if (source == tab_strip_model_->GetActiveWebContents()) {
    GetStatusBubble()->MouseMoved(location, exited);
    if (exited)
      GetStatusBubble()->SetURL(GURL());
  }
}
