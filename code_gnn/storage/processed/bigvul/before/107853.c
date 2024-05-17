void PrintWebViewHelper::didStopLoading() {
  DCHECK(print_pages_params_.get() != NULL);
  PrintPages(*print_pages_params_.get(), print_web_view_->mainFrame(), NULL);
}
