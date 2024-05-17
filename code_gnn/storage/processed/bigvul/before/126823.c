void BrowserView::LoadingAnimationCallback() {
  base::TimeTicks now = base::TimeTicks::Now();
  if (!last_animation_time_.is_null()) {
    UMA_HISTOGRAM_TIMES(
        "Tabs.LoadingAnimationTime",
        now - last_animation_time_);
  }
  last_animation_time_ = now;
  if (browser_->is_type_tabbed()) {
    tabstrip_->UpdateLoadingAnimations();
  } else if (ShouldShowWindowIcon()) {
    WebContents* web_contents = chrome::GetActiveWebContents(browser_.get());
    frame_->UpdateThrobber(web_contents && web_contents->IsLoading());
  }
}
