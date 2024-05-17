void RenderView::didUpdateLayout(WebFrame* frame) {
  if (!send_preferred_size_changes_ || !webview())
    return;

  if (check_preferred_size_timer_.IsRunning())
    return;
  check_preferred_size_timer_.Start(TimeDelta::FromMilliseconds(0), this,
                                    &RenderView::CheckPreferredSize);
}
