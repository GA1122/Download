void Browser::UpdatePreferredSize(WebContents* source,
                                  const gfx::Size& pref_size) {
  window_->UpdatePreferredSize(source, pref_size);
}
