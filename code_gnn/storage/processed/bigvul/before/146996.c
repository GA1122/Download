float WebLocalFrameImpl::GetPrintPageShrink(int page) {
  DCHECK(print_context_);
  DCHECK_GE(page, 0);
  return print_context_->GetPageShrink(page);
}
