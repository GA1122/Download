void WebContentsImpl::ResetOverrideEncoding() {
  canonical_encoding_.clear();
  Send(new ViewMsg_ResetPageEncodingToDefault(GetRoutingID()));
}
