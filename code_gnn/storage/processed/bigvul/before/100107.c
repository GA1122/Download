int BrowserActionsContainer::ClampToNearestIconCount(int pixelWidth) const {
  int icon_width = (kButtonSize + kBrowserActionButtonPadding);

  int extras = WidthOfNonIconArea();

  size_t icon_count = 0u;
  if (pixelWidth >= 0) {
    int icon_area = pixelWidth - extras;
    icon_area = std::max(0, icon_area);

    icon_area += kBrowserActionButtonPadding - 1;

    icon_count = icon_area / icon_width;

    if (icon_count > browser_action_views_.size())
      icon_count = browser_action_views_.size();
    else if (icon_count == 0)
      extras = ContainerMinSize();   
  } else {
    icon_count = browser_action_views_.size();
  }

  int returning = extras + (icon_count * icon_width);
   return returning;
 }
