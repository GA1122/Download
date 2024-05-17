void WebFrameLoaderClient::windowObjectCleared() {
  if (webframe_->client())
    webframe_->client()->didClearWindowObject(webframe_);

  WebViewImpl* webview = webframe_->GetWebViewImpl();
  if (webview) {
    WebDevToolsAgentImpl* tools_agent = webview->GetWebDevToolsAgentImpl();
    if (tools_agent)
      tools_agent->WindowObjectCleared(webframe_);
  }
}
