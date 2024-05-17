FaviconSource::FaviconSource(Profile* profile, IconType type)
    : DataSource(type == FAVICON ? chrome::kChromeUIFaviconHost :
                     chrome::kChromeUITouchIconHost,
                 MessageLoop::current()),
      profile_(profile->GetOriginalProfile()),
      icon_types_(type == FAVICON ? history::FAVICON :
          history::TOUCH_PRECOMPOSED_ICON | history::TOUCH_ICON |
          history::FAVICON) {
}
