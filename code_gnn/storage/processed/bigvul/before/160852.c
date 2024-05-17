static void ConfigueCompositingWebView(WebSettings* settings) {
  settings->SetAcceleratedCompositingEnabled(true);
  settings->SetPreferCompositingToLCDTextEnabled(true);
}
