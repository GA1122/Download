void PaymentHandlerWebFlowViewController::LoadProgressChanged(
    content::WebContents* source,
    double progress) {
  DCHECK(source == web_contents());

  progress_bar_->SetValue(progress);

  if (progress == 1.0 && show_progress_bar_) {
    show_progress_bar_ = false;
    UpdateHeaderContentSeparatorView();
    return;
  }

  if (progress < 1.0 && !show_progress_bar_) {
    show_progress_bar_ = true;
    UpdateHeaderContentSeparatorView();
    return;
  }
}
