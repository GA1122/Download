WebString WebLocalFrameImpl::PageProperty(const WebString& property_name,
                                          int page_index) {
  DCHECK(print_context_);
  return print_context_->PageProperty(GetFrame(), property_name.Utf8().data(),
                                      page_index);
}
