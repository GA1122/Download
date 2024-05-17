void RenderFrameImpl::DidChangeManifest() {
  for (auto& observer : observers_)
    observer.DidChangeManifest();
}
