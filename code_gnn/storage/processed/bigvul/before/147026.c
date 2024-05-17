void WebLocalFrameImpl::LoadRequest(const WebURLRequest& request) {
  Load(request, WebFrameLoadType::kStandard, WebHistoryItem(),
       kWebHistoryDifferentDocumentLoad, false);
}
