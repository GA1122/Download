void RenderFrameImpl::OnNavigate(
    const CommonNavigationParams& common_params,
    const StartNavigationParams& start_params,
    const RequestNavigationParams& request_params) {
  DCHECK(!base::CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kEnableBrowserSideNavigation));
  TRACE_EVENT2("navigation", "RenderFrameImpl::OnNavigate", "id", routing_id_,
               "url", common_params.url.possibly_invalid_spec());
  NavigateInternal(common_params, start_params, request_params,
                   scoped_ptr<StreamOverrideParameters>());
}
