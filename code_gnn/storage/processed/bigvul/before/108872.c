WebFrame* RenderViewImpl::GetChildFrame(const string16& xpath) const {
  if (xpath.empty())
    return webview()->mainFrame();

  std::vector<string16> xpaths;
  base::SplitString(xpath, '\n', &xpaths);

  WebFrame* frame = webview()->mainFrame();
  for (std::vector<string16>::const_iterator i = xpaths.begin();
       frame && i != xpaths.end(); ++i) {
    frame = frame->findChildByExpression(*i);
  }

  return frame;
}
