void RenderViewTest::Reload(const GURL& url) {
  CommonNavigationParams common_params(
      url, Referrer(), ui::PAGE_TRANSITION_LINK, FrameMsg_Navigate_Type::RELOAD,
      true, false, base::TimeTicks(),
      FrameMsg_UILoadMetricsReportType::NO_REPORT, GURL(), GURL());
  RenderViewImpl* impl = static_cast<RenderViewImpl*>(view_);
  TestRenderFrame* frame =
      static_cast<TestRenderFrame*>(impl->GetMainRenderFrame());
  frame->Navigate(common_params, StartNavigationParams(),
                  RequestNavigationParams());
  FrameLoadWaiter(frame).Wait();
}
