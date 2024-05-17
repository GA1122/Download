void DevToolsUIBindings::FrontendWebContentsObserver::DidFinishNavigation(
    content::NavigationHandle* navigation_handle) {
  if (navigation_handle->IsInMainFrame() && navigation_handle->HasCommitted())
    devtools_bindings_->DidNavigateMainFrame();
}
