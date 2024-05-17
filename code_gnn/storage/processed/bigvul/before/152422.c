void RenderFrameImpl::OnClearFocusedElement() {
  if (auto* webview = render_view_->GetWebView())
    webview->ClearFocusedElement();
}
