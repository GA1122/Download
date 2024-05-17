void Browser::ActivateContents(WebContents* contents) {
  chrome::ActivateTabAt(this, tab_strip_model_->GetIndexOfWebContents(contents),
                        false);
  window_->Activate();
}
