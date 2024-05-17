blink::WebLocalFrame* FrameReference::GetFrame() {
  if (view_ == nullptr || frame_ == nullptr)
    return nullptr;
  for (blink::WebFrame* frame = view_->MainFrame(); frame != nullptr;
       frame = frame->TraverseNext()) {
    if (frame == frame_)
      return frame_;
  }
  return nullptr;
}
