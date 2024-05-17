BrowserActionView* BrowserActionsContainer::GetBrowserActionView(
    Extension* extension) {
  for (std::vector<BrowserActionView*>::iterator iter =
       browser_action_views_.begin(); iter != browser_action_views_.end();
       ++iter) {
    if ((*iter)->button()->extension() == extension)
      return *iter;
  }

  return NULL;
}
