void TabLoader::TabIsLoading(NavigationController* controller) {
  DCHECK(controller);
  DCHECK(find(tabs_loading_.begin(), tabs_loading_.end(), controller) ==
         tabs_loading_.end());
  tabs_loading_.insert(controller);
  RenderWidgetHost* render_widget_host = GetRenderWidgetHost(controller);
  DCHECK(render_widget_host);
  render_widget_hosts_loading_.insert(render_widget_host);
  RegisterForNotifications(controller);
}
