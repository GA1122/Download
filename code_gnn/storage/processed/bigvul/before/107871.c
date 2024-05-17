bool WebViewPlugin::canHandleRequest(WebFrame* frame,
                                     const WebURLRequest& request) {
  return GURL(request.url()).SchemeIs("chrome");
}
