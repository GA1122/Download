void RenderFrameHostImpl::ResumeBlockedRequestsForFrame() {
  if (base::FeatureList::IsEnabled(network::features::kNetworkService)) {
    ForEachFrame(
        this, base::BindRepeating([](RenderFrameHostImpl* render_frame_host) {
          if (render_frame_host->frame_)
            render_frame_host->frame_->ResumeBlockedRequests();
        }));
  } else {
    NotifyForEachFrameFromUI(
        this, base::BindRepeating(
                  &ResourceDispatcherHostImpl::ResumeBlockedRequestsForRoute));
  }
}
