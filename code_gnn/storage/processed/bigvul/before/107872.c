WebURLError WebViewPlugin::cancelledError(WebFrame* frame,
                                          const WebURLRequest& request) {
  WebURLError error;
  error.domain = "WebViewPlugin";
  error.reason = -1;
  error.unreachableURL = request.url();
  return error;
}
