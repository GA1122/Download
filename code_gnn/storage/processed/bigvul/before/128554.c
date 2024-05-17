void Instance::DocumentPaintOccurred() {
  if (painted_first_page_)
    return;

  painted_first_page_ = true;
  UpdateToolbarPosition(false);
  toolbar_->Splash(kToolbarSplashTimeoutMs);

  if (engine_->GetNumberOfPages() > 1)
    show_page_indicator_ = true;
  else
    show_page_indicator_ = false;

  if (v_scrollbar_.get() && show_page_indicator_) {
    page_indicator_.set_current_page(GetPageNumberToDisplay());
    page_indicator_.Splash(kToolbarSplashTimeoutMs,
                           kPageIndicatorInitialFadeTimeoutMs);
  }
}
