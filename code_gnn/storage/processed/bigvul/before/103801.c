void RenderView::OnCSSInsertRequest(const std::wstring& frame_xpath,
                                    const std::string& css,
                                    const std::string& id) {
  WebFrame* frame = GetChildFrame(frame_xpath);
  if (!frame)
    return;

  frame->document().insertStyleText(WebString::fromUTF8(css),
                                    WebString::fromUTF8(id));
}
