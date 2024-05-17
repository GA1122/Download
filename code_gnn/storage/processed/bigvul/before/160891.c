void WebViewTest::TestAutoResize(
    const WebSize& min_auto_resize,
    const WebSize& max_auto_resize,
    const std::string& page_width,
    const std::string& page_height,
    int expected_width,
    int expected_height,
    HorizontalScrollbarState expected_horizontal_state,
    VerticalScrollbarState expected_vertical_state) {
  AutoResizeWebViewClient client;
  std::string url =
      base_url_ + "specify_size.html?" + page_width + ":" + page_height;
  URLTestHelpers::RegisterMockedURLLoad(
      ToKURL(url), testing::CoreTestDataPath("specify_size.html"));
  WebViewImpl* web_view =
      web_view_helper_.InitializeAndLoad(url, nullptr, &client);
  client.GetTestData().SetWebView(web_view);

  WebLocalFrameImpl* frame = web_view->MainFrameImpl();
  LocalFrameView* frame_view = frame->GetFrame()->View();
  frame_view->UpdateLayout();
  EXPECT_FALSE(frame_view->LayoutPending());
  EXPECT_FALSE(frame_view->NeedsLayout());

  web_view->EnableAutoResizeMode(min_auto_resize, max_auto_resize);
  EXPECT_TRUE(frame_view->LayoutPending());
  EXPECT_TRUE(frame_view->NeedsLayout());
  frame_view->UpdateLayout();

  EXPECT_TRUE(frame->GetFrame()->GetDocument()->IsHTMLDocument());

  EXPECT_EQ(expected_width, client.GetTestData().Width());
  EXPECT_EQ(expected_height, client.GetTestData().Height());

#if !defined(OS_ANDROID)
  EXPECT_EQ(expected_horizontal_state,
            client.GetTestData().GetHorizontalScrollbarState());
  EXPECT_EQ(expected_vertical_state,
            client.GetTestData().GetVerticalScrollbarState());
#endif

  web_view_helper_.Reset();
}
