  void TestRenderViewGone(base::TerminationStatus status, int error_code) {
    RenderViewGone(GetRenderViewHostForTesting(), status, error_code);
  }
