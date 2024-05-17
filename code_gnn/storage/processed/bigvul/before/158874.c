void Browser::VisibleSecurityStateChanged(WebContents* source) {
  DCHECK(source);
  if (tab_strip_model_->GetActiveWebContents() == source) {
    UpdateToolbar(false);

    if (hosted_app_controller_)
      hosted_app_controller_->UpdateLocationBarVisibility(true);
  }
}
