ChromeContentRendererClient::~ChromeContentRendererClient() {
  chrome::UninitializePPAPI();
}