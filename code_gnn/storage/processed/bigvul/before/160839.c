blink::WebString WebViewTestClient::AcceptLanguages() {
  return blink::WebString::FromUTF8(test_runner()->GetAcceptLanguages());
}
