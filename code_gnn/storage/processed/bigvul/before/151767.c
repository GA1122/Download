void Browser::VisibleSecurityStateChanged(WebContents* source) {
  DCHECK(source);
  if (tab_strip_model_->GetActiveWebContents() == source)
    UpdateToolbar(false);

  SecurityStateTabHelper* helper =
      SecurityStateTabHelper::FromWebContents(source);
  helper->VisibleSecurityStateChanged();
}
