void Document::NavigateLocalAdsFrames() {
  DCHECK(frame_);
  for (Frame* child = frame_->Tree().FirstChild(); child;
       child = child->Tree().TraverseNext(frame_)) {
    if (auto* child_local_frame = DynamicTo<LocalFrame>(child)) {
      if (child_local_frame->IsAdSubframe()) {
        child_local_frame->Navigate(
            FrameLoadRequest(this, ResourceRequest(BlankURL())),
            WebFrameLoadType::kStandard);
      }
    }
  }
}
