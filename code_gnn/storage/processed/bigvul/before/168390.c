std::unique_ptr<Browser> CreateBrowserWithTestWindowForParams(
    Browser::CreateParams* params) {
  TestBrowserWindow* window = new TestBrowserWindow;
  new TestBrowserWindowOwner(window);
  params->window = window;
  return std::make_unique<Browser>(*params);
}
