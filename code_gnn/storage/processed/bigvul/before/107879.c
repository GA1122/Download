void WebViewPlugin::didReceiveResponse(WebFrame* frame,
                                       unsigned identifier,
                                       const WebURLResponse& response) {
  WebFrameClient::didReceiveResponse(frame, identifier, response);
}
