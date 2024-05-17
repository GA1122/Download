HomeButton::HomeButton(
    views::ButtonListener* listener,
    Browser* browser)
    : ToolbarButton(listener, NULL),
      browser_(browser) {
}
