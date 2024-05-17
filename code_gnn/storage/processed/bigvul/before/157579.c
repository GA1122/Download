  void OnSetWindowBounds(
      std::unique_ptr<browser::SetWindowBoundsResult> result) {
    GetWindowBounds(base::Bind(
        &HeadlessDevToolsClientChangeWindowBoundsTest::OnGetWindowBounds,
        base::Unretained(this)));
  }
