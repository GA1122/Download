void ChromeClientImpl::SetBrowserControlsState(float top_height,
                                               float bottom_height,
                                               bool shrinks_layout) {
  WebSize size = web_view_->Size();
  if (shrinks_layout)
    size.height -= top_height + bottom_height;

  web_view_->ResizeWithBrowserControls(size, top_height, bottom_height,
                                       shrinks_layout);
}
