blink::WebLocalFrame* FrameReference::GetFrame() {
  if (view_ == NULL || frame_ == NULL)
    return NULL;
  for (blink::WebFrame* frame = view_->mainFrame(); frame != NULL;
       frame = frame->traverseNext(false)) {
    if (frame == frame_)
      return frame_;
  }
  return NULL;
}
