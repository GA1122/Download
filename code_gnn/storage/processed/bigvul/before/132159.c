RenderViewTest::RenderViewTest()
    : view_(NULL) {
  RenderFrameImpl::InstallCreateHook(&TestRenderFrame::CreateTestRenderFrame);
}
