void Browser::OnZoomChanged(content::WebContents* source,
                            bool can_show_bubble) {
  if (source == chrome::GetActiveWebContents(this)) {
    window_->ZoomChangedForActiveTab(can_show_bubble && window_->IsActive());
  }
}
