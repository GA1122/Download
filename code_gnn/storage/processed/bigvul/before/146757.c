ReferrerPolicy Document::GetReferrerPolicy() const {
  ReferrerPolicy policy = ExecutionContext::GetReferrerPolicy();
  if (!frame_ || policy != kReferrerPolicyDefault || !IsSrcdocDocument()) {
    return policy;
  }
  LocalFrame* frame = ToLocalFrame(frame_->Tree().Parent());
  DCHECK(frame);
  return frame->GetDocument()->GetReferrerPolicy();
}
