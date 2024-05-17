bool Browser::SupportsWindowFeatureImpl(WindowFeature feature,
                                        bool check_fullscreen) const {
  bool hide_ui_for_fullscreen = false;
#if !defined(OS_MACOSX)
  hide_ui_for_fullscreen = check_fullscreen && window_ &&
      window_->IsFullscreen();
#endif

  unsigned int features = FEATURE_INFOBAR | FEATURE_SIDEBAR;

#if !defined(OS_CHROMEOS)
  features |= FEATURE_DOWNLOADSHELF;
#endif   

  if (type() == TYPE_NORMAL) {
    features |= FEATURE_BOOKMARKBAR;
  }

  if (!hide_ui_for_fullscreen) {
    if (type() != TYPE_NORMAL)
      features |= FEATURE_TITLEBAR;

    if (type() == TYPE_NORMAL)
      features |= FEATURE_TABSTRIP;

    if (type() == TYPE_NORMAL)
      features |= FEATURE_TOOLBAR;

    if ((type() & Browser::TYPE_APP) == 0)
      features |= FEATURE_LOCATIONBAR;
  }
  return !!(features & feature);
}
