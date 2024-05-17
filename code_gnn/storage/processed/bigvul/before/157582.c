  void SetWindowState(
      const browser::WindowState state,
      base::Callback<void(std::unique_ptr<browser::SetWindowBoundsResult>)>
          callback) {
    std::unique_ptr<browser::Bounds> bounds =
        browser::Bounds::Builder().SetWindowState(state).Build();
    int window_id = HeadlessWebContentsImpl::From(web_contents_)->window_id();
    std::unique_ptr<browser::SetWindowBoundsParams> params =
        browser::SetWindowBoundsParams::Builder()
            .SetWindowId(window_id)
            .SetBounds(std::move(bounds))
            .Build();
    browser_devtools_client_->GetBrowser()->GetExperimental()->SetWindowBounds(
        std::move(params), callback);
  }
