void FrameReference::Reset(blink::WebLocalFrame* frame) {
  if (frame) {
    view_ = frame->View();
    DCHECK(view_);
    frame_ = frame;
  } else {
    view_ = nullptr;
    frame_ = nullptr;
  }
}
