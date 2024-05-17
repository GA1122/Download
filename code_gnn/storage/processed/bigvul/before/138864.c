TestRenderViewHost* RenderViewHostImplTestHarness::pending_test_rvh() {
  return contents()->GetPendingMainFrame() ?
      contents()->GetPendingMainFrame()->GetRenderViewHost() :
      nullptr;
}
