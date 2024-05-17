blink::BlameContext* RenderFrameImpl::GetFrameBlameContext() {
  DCHECK(blame_context_);
  return blame_context_.get();
}
