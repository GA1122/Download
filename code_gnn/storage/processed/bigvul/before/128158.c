void ExecuteJavaScript(const std::string& javascript) {
  auto rvh = GetRenderViewHost();
  if (!rvh)
    return;
  rvh->GetMainFrame()->ExecuteJavaScript(base::UTF8ToUTF16(javascript));
}
