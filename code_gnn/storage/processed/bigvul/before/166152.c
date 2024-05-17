bool RenderFrameHostImpl::PrepareForInnerWebContentsAttach() {
  DCHECK(MimeHandlerViewMode::UsesCrossProcessFrame());
  if (IsCrossProcessSubframe() || !GetParent())
    return false;
  ResetNavigationRequests();
  ResetLoadingState();
  is_attaching_inner_delegate_ = true;
  return true;
}
