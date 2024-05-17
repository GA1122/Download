float WebLocalFrameImpl::PrintPage(int page, cc::PaintCanvas* canvas) {
  DCHECK(print_context_);
  DCHECK_GE(page, 0);
  DCHECK(GetFrame());
  DCHECK(GetFrame()->GetDocument());

  return print_context_->SpoolSinglePage(canvas, page);
}
