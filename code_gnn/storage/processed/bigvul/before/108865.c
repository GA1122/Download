static WebKit::WebFrame* FindFrameByID(WebKit::WebFrame* root, int frame_id) {
  for (WebFrame* frame = root; frame; frame = frame->traverseNext(false)) {
    if (frame->identifier() == frame_id)
      return frame;
  }
  return NULL;
}
