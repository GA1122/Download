void RenderViewImpl::ApplyNewSizeForWidget(const gfx::Size& old_size,
                                           const gfx::Size& new_size) {
  if (webview()) {
    if (new_size != old_size)
      webview()->HidePopups();

    if (send_preferred_size_changes_ &&
        webview()->MainFrame()->IsWebLocalFrame()) {
      webview()->MainFrame()->ToWebLocalFrame()->SetCanHaveScrollbars(
          ShouldDisplayScrollbars(new_size.width(), new_size.height()));
    }
  }
}
