void TestRenderViewHost::TestOnUpdateStateWithFile(
    const base::FilePath& file_path) {
  PageState state = PageState::CreateForTesting(GURL("http://www.google.com"),
                                                false, "data", &file_path);
  static_cast<RenderFrameHostImpl*>(GetMainFrame())->OnUpdateState(state);
}
