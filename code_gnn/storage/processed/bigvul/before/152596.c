RenderFrameImpl* TestRenderFrame::CreateTestRenderFrame(
    RenderFrameImpl::CreateParams params) {
  return new TestRenderFrame(std::move(params));
}
