AppLoadedInTabSource ClassifyAppLoadedInTabSource(
    const GURL& opener_url,
    const extensions::Extension* target_platform_app) {
  if (opener_url.SchemeIs(extensions::kExtensionScheme)) {
    if (opener_url.host_piece() == target_platform_app->id()) {
      if (opener_url ==
          extensions::BackgroundInfo::GetBackgroundURL(target_platform_app)) {
        return APP_LOADED_IN_TAB_SOURCE_BACKGROUND_PAGE;
      } else {
        return APP_LOADED_IN_TAB_SOURCE_APP;
      }
    }
    return APP_LOADED_IN_TAB_SOURCE_OTHER_EXTENSION;
  }
  return APP_LOADED_IN_TAB_SOURCE_OTHER;
}
