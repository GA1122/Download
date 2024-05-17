void RenderFrameImpl::NotifyObserversOfNavigationCommit(bool is_new_navigation,
                                                        bool is_same_document) {
  for (auto& observer : render_view_->observers_)
    observer.DidCommitProvisionalLoad(frame_, is_new_navigation);
  {
    SCOPED_UMA_HISTOGRAM_TIMER("RenderFrameObservers.DidCommitProvisionalLoad");
    for (auto& observer : observers_) {
      observer.DidCommitProvisionalLoad(is_new_navigation, is_same_document);
    }
  }
}
