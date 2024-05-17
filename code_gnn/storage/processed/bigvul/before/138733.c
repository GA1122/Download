void RenderFrameHostImpl::OnBeginNavigation(
    const CommonNavigationParams& common_params,
    const BeginNavigationParams& begin_params) {
  CHECK(IsBrowserSideNavigationEnabled());
  if (!is_active())
    return;
  CommonNavigationParams validated_params = common_params;
  GetProcess()->FilterURL(false, &validated_params.url);

  BeginNavigationParams validated_begin_params = begin_params;
  GetProcess()->FilterURL(true, &validated_begin_params.searchable_form_url);

  if (waiting_for_init_) {
    pendinging_navigate_ = base::MakeUnique<PendingNavigation>(
        validated_params, validated_begin_params);
    return;
  }

  frame_tree_node()->navigator()->OnBeginNavigation(
      frame_tree_node(), validated_params, validated_begin_params);
}
