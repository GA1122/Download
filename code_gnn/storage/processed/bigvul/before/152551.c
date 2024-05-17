void RenderFrameImpl::UpdateEncoding(WebFrame* frame,
                                     const std::string& encoding_name) {
  if (!frame->Parent())
    GetFrameHost()->UpdateEncoding(encoding_name);
}
