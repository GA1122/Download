void WebFrameLoaderClient::dispatchDidCommitLoad() {
  WebViewImpl* webview = webframe_->GetWebViewImpl();
  bool is_new_navigation;
  webview->DidCommitLoad(&is_new_navigation);

  if (webframe_->client()) {
    webframe_->client()->didCommitProvisionalLoad(
        webframe_, is_new_navigation);
  }

  WebDevToolsAgentImpl* tools_agent = webview->GetWebDevToolsAgentImpl();
  if (tools_agent) {
    tools_agent->DidCommitLoadForFrame(webview, webframe_, is_new_navigation);
  }
}
