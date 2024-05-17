bool TabStrip::SizeTabButtonToTopOfTabStrip() {
  views::Widget* widget = GetWidget();
  return browser_defaults::kSizeTabButtonToTopOfTabStrip ||
      (widget && (widget->IsMaximized() || widget->IsFullscreen()));
}
