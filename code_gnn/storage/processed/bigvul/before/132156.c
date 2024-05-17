void RenderViewTest::LoadHTML(const char* html) {
  std::string url_str = "data:text/html;charset=utf-8,";
  url_str.append(html);
  GURL url(url_str);
  WebURLRequest request(url);
  request.setCheckForBrowserSideNavigation(false);
  GetMainFrame()->loadRequest(request);
  FrameLoadWaiter(view_->GetMainRenderFrame()).Wait();
}
