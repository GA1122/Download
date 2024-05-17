PassRefPtr<Frame> WebFrameLoaderClient::createFrame(
    const KURL& url,
    const String& name,
    HTMLFrameOwnerElement* owner_element,
    const String& referrer,
    bool allows_scrolling,
    int margin_width,
    int margin_height) {
  FrameLoadRequest frame_request(ResourceRequest(url, referrer), name);
  return webframe_->CreateChildFrame(frame_request, owner_element);
}
