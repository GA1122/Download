void RenderFrameImpl::OnCommitNavigation(
    const ResourceResponseHead& response,
    const GURL& stream_url,
    const CommonNavigationParams& common_params,
    const RequestNavigationParams& request_params) {
  CHECK(base::CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kEnableBrowserSideNavigation));
  scoped_ptr<StreamOverrideParameters> stream_override(
      new StreamOverrideParameters());
  stream_override->stream_url = stream_url;
  stream_override->response = response;

  NavigateInternal(common_params, StartNavigationParams(), request_params,
                   stream_override.Pass());
}
