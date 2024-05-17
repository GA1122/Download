void RenderFrameImpl::NotifyObserversOfNavigationCommit(
    bool is_new_navigation,
    bool is_same_document,
    ui::PageTransition transition) {
  for (auto& observer : render_view_->observers_)
    observer.DidCommitProvisionalLoad(frame_, is_new_navigation);
  for (auto& observer : observers_)
    observer.DidCommitProvisionalLoad(is_same_document, transition);
}
