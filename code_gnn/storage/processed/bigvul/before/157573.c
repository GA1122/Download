  void OnGetWindowState(
      std::unique_ptr<browser::GetWindowBoundsResult> result) {
    HeadlessBrowser::Options::Builder builder;
    const HeadlessBrowser::Options kDefaultOptions = builder.Build();
    CheckWindowBounds(gfx::Rect(kDefaultOptions.window_size), state_,
                      std::move(result));
    FinishAsynchronousTest();
  }
