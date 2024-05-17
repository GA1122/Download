bool VRDisplay::IsPresentationFocused() {
  if (!navigator_vr_)
    return false;

  if (navigator_vr_->IsFocused())
    return true;

  auto doc = navigator_vr_->GetDocument();
  if (!doc)
    return false;


  Frame* frame = doc->GetFrame();
  for (; frame; frame = frame->Tree().Parent()) {
    if (!frame->IsLocalFrame())
      break;
    auto frame_doc = ToLocalFrame(frame)->GetDocument();
    if (frame_doc && frame_doc->hasFocus()) {
      DVLOG(3) << __FUNCTION__ << ": a parent frame is focused";
      return true;
    }
  }

  return false;
}
