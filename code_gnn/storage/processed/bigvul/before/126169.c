void Browser::ContentsMouseEvent(
    WebContents* source, const gfx::Point& location, bool motion) {
  if (!GetStatusBubble())
    return;

  if (source == chrome::GetActiveWebContents(this)) {
    GetStatusBubble()->MouseMoved(location, !motion);
    if (!motion)
      GetStatusBubble()->SetURL(GURL(), std::string());
  }
}
