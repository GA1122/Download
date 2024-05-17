bool WebLocalFrameImpl::UsePrintingLayout() const {
  return print_context_ ? print_context_->use_printing_layout() : false;
}
