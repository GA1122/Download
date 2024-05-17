blink::WebView* WebViewTestClient::CreateView(
    blink::WebLocalFrame* frame,
    const blink::WebURLRequest& request,
    const blink::WebWindowFeatures& features,
    const blink::WebString& frame_name,
    blink::WebNavigationPolicy policy,
    bool suppress_opener,
    blink::WebSandboxFlags sandbox_flags) {
  if (test_runner()->shouldDumpNavigationPolicy()) {
    delegate()->PrintMessage("Default policy for createView for '" +
                             URLDescription(request.Url()) + "' is '" +
                             WebNavigationPolicyToString(policy) + "'\n");
  }

  if (!test_runner()->canOpenWindows())
    return nullptr;
  if (test_runner()->shouldDumpCreateView())
    delegate()->PrintMessage(std::string("createView(") +
                             URLDescription(request.Url()) + ")\n");

  return reinterpret_cast<blink::WebView*>(0xdeadbeef);
}
