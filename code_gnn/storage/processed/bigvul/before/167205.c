GlobalConfirmInfoBar::GlobalConfirmInfoBar(
    std::unique_ptr<ConfirmInfoBarDelegate> delegate)
    : delegate_(std::move(delegate)),
      browser_tab_strip_tracker_(this, nullptr, nullptr),
      is_closing_(false),
      weak_factory_(this) {
  browser_tab_strip_tracker_.Init();
}
