bool DidExtensionViewsStopLoading(ExtensionProcessManager* manager) {
  ExtensionProcessManager::ViewSet all_views = manager->GetAllViews();
  for (ExtensionProcessManager::ViewSet::const_iterator iter =
           all_views.begin();
       iter != all_views.end(); ++iter) {
    if ((*iter)->IsLoading())
      return false;
  }
  return true;
}
