void RenderThreadImpl::CreateFrame(mojom::CreateFrameParamsPtr params) {
  base::debug::SetCrashKeyValue("newframe_routing_id",
                                base::IntToString(params->routing_id));
  base::debug::SetCrashKeyValue("newframe_proxy_id",
                                base::IntToString(params->proxy_routing_id));
  base::debug::SetCrashKeyValue("newframe_opener_id",
                                base::IntToString(params->opener_routing_id));
  base::debug::SetCrashKeyValue("newframe_parent_id",
                                base::IntToString(params->parent_routing_id));
  base::debug::SetCrashKeyValue("newframe_widget_id",
                                base::IntToString(
                                    params->widget_params->routing_id));
  base::debug::SetCrashKeyValue("newframe_widget_hidden",
                                params->widget_params->hidden ? "yes" : "no");
  base::debug::SetCrashKeyValue("newframe_replicated_origin",
                                params->replication_state.origin.Serialize());
  CompositorDependencies* compositor_deps = this;
  RenderFrameImpl::CreateFrame(
      params->routing_id, std::move(params->interface_provider),
      params->proxy_routing_id, params->opener_routing_id,
      params->parent_routing_id, params->previous_sibling_routing_id,
      params->devtools_frame_token, params->replication_state, compositor_deps,
      *params->widget_params, params->frame_owner_properties);
}
