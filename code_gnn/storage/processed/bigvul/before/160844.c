blink::WebSpeechRecognizer* WebViewTestClient::SpeechRecognizer() {
  return test_runner()->getMockWebSpeechRecognizer();
}
