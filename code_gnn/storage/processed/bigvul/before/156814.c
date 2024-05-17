void Document::NavigateLocalAdsFrames() {
  DCHECK(frame_);
  for (Frame* child = frame_->Tree().FirstChild(); child;
       child = child->Tree().TraverseNext(frame_)) {
    if (child->IsLocalFrame()) {
      if (ToLocalFrame(child)->IsAdSubframe()) {
        ToLocalFrame(child)->Navigate(
            FrameLoadRequest(this, ResourceRequest(BlankURL())),
            WebFrameLoadType::kStandard);
      }
    }
  }
}
