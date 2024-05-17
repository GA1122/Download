network::mojom::ReferrerPolicy Document::GetReferrerPolicy() const {
  network::mojom::ReferrerPolicy policy = ExecutionContext::GetReferrerPolicy();
  if (!frame_ || policy != network::mojom::ReferrerPolicy::kDefault ||
      !IsSrcdocDocument()) {
    return policy;
  }
  LocalFrame* frame = To<LocalFrame>(frame_->Tree().Parent());
  return frame->GetDocument()->GetReferrerPolicy();
}
