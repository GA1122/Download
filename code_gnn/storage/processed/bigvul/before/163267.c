void BrowserMainLoop::Init() {
  TRACE_EVENT0("startup", "BrowserMainLoop::Init");

  parts_.reset(
      GetContentClient()->browser()->CreateBrowserMainParts(parameters_));
}
