int BrowserActionsContainer::IconCountToWidth(int icons) const {
  DCHECK(icons >= 0);
  if (icons == 0)
    return ContainerMinSize();

  int icon_width = kButtonSize + kBrowserActionButtonPadding;

  return WidthOfNonIconArea() + (icons * icon_width);
}
