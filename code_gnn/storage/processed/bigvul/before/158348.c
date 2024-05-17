void RenderWidgetHostImpl::RequestCompositionUpdates(bool immediate_request,
                                                     bool monitor_updates) {
  if (!immediate_request && monitor_updates == monitoring_composition_info_)
    return;
  monitoring_composition_info_ = monitor_updates;
  GetWidgetInputHandler()->RequestCompositionUpdates(immediate_request,
                                                     monitor_updates);
}
