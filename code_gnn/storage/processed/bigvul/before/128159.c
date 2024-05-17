void ExecuteJavaScriptWithCallback(const std::string& javascript,
                                   const JavaScriptResultCallback& callback) {
  auto rvh = GetRenderViewHost();
  if (!rvh)
    return;
  rvh->GetMainFrame()->ExecuteJavaScript(base::UTF8ToUTF16(javascript),
                                         callback);
}
