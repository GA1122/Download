void RenderFrameHostImpl::OnCrossDocumentCommitProcessed(
    int64_t navigation_id,
    blink::mojom::CommitResult result) {
  DCHECK_NE(blink::mojom::CommitResult::RestartCrossDocument, result);
  if (result == blink::mojom::CommitResult::Ok) {
    auto find_request = navigation_requests_.find(navigation_id);
    if (find_request != navigation_requests_.end()) {
      navigation_request_ = std::move(find_request->second);
    } else {
      DCHECK(is_attaching_inner_delegate_);
    }
  }
  navigation_requests_.erase(navigation_id);
}
