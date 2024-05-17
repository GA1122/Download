WebLocalFrame* RenderViewTest::GetMainFrame() {
  return view_->GetWebView()->MainFrame()->ToWebLocalFrame();
}
