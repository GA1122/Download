  void OnGetWindowBounds(
      std::unique_ptr<browser::GetWindowBoundsResult> result) {
    CheckWindowBounds(gfx::Rect(100, 200, 300, 400),
                      browser::WindowState::NORMAL, std::move(result));
    FinishAsynchronousTest();
  }
