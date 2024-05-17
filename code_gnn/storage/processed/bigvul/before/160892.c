void WebViewTest::TestInputMode(WebTextInputMode expected_input_mode,
                                const std::string& html_file) {
  RegisterMockedHttpURLLoad(html_file);
  WebViewImpl* web_view_impl =
      web_view_helper_.InitializeAndLoad(base_url_ + html_file);
  web_view_impl->SetInitialFocus(false);
  EXPECT_EQ(expected_input_mode, web_view_impl->MainFrameImpl()
                                     ->GetInputMethodController()
                                     ->TextInputInfo()
                                     .input_mode);
}
