void RenderViewTest::LoadHTML(const char* html) {
  std::string url_string = "data:text/html;charset=utf-8,";
  url_string.append(html);
  GURL url(url_string);
  WebURLRequest request(url);
  request.SetCheckForBrowserSideNavigation(false);
  GetMainFrame()->LoadRequest(request);
  FrameLoadWaiter(view_->GetMainRenderFrame()).Wait();
  view_->GetWebView()->UpdateAllLifecyclePhases();
}
