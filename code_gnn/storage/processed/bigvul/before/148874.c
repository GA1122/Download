void RenderFrameHostManager::DidCreateNavigationRequest(
    NavigationRequest* request) {
  CHECK(IsBrowserSideNavigationEnabled());
  RenderFrameHostImpl* dest_rfh = GetFrameHostForNavigation(*request);
  DCHECK(dest_rfh);
  request->set_associated_site_instance_type(
      dest_rfh == render_frame_host_.get()
          ? NavigationRequest::AssociatedSiteInstanceType::CURRENT
          : NavigationRequest::AssociatedSiteInstanceType::SPECULATIVE);
}
