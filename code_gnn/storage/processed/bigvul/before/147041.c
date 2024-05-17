float WebLocalFrameImpl::PrintPage(int page, WebCanvas* canvas) {
  DCHECK(print_context_);
  DCHECK_GE(page, 0);
  DCHECK(GetFrame());
  DCHECK(GetFrame()->GetDocument());

  return print_context_->SpoolSinglePage(canvas, page);
}
