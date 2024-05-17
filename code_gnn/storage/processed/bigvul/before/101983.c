int PrintWebViewHelper::PrintPreviewContext::total_page_count() const {
  DCHECK(IsReadyToRender());
  return total_page_count_;
}
