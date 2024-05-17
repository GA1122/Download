void OmniboxViewViews::SetGrayTextAutocompletion(const base::string16& input) {
#if defined(OS_WIN) || defined(USE_AURA)
  location_bar_view_->SetGrayTextAutocompletion(input);
#endif
}
