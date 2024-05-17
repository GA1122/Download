void WebViewTest::TestTextInputType(WebTextInputType expected_type,
                                    const std::string& html_file) {
  RegisterMockedHttpURLLoad(html_file);
  WebViewImpl* web_view =
      web_view_helper_.InitializeAndLoad(base_url_ + html_file);
  WebInputMethodController* controller =
      web_view->MainFrameImpl()->GetInputMethodController();
  EXPECT_EQ(kWebTextInputTypeNone, controller->TextInputType());
  EXPECT_EQ(kWebTextInputTypeNone, controller->TextInputInfo().type);
  web_view->SetInitialFocus(false);
  EXPECT_EQ(expected_type, controller->TextInputType());
  EXPECT_EQ(expected_type, controller->TextInputInfo().type);
  web_view->ClearFocusedElement();
  EXPECT_EQ(kWebTextInputTypeNone, controller->TextInputType());
  EXPECT_EQ(kWebTextInputTypeNone, controller->TextInputInfo().type);
}
