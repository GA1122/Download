void RenderFrameHostImpl::SetNavigationsSuspended(
    bool suspend,
    const base::TimeTicks& proceed_time) {
  DCHECK(navigations_suspended_ != suspend);

  navigations_suspended_ = suspend;
  if (navigations_suspended_) {
    TRACE_EVENT_ASYNC_BEGIN0("navigation",
                             "RenderFrameHostImpl navigation suspended", this);
  } else {
    TRACE_EVENT_ASYNC_END0("navigation",
                           "RenderFrameHostImpl navigation suspended", this);
  }

  if (!suspend && suspended_nav_params_) {
    ResetWaitingState();

    DCHECK(!proceed_time.is_null());
    suspended_nav_params_->common_params.navigation_start = proceed_time;
    SendNavigateMessage(suspended_nav_params_->common_params,
                        suspended_nav_params_->start_params,
                        suspended_nav_params_->request_params);
    suspended_nav_params_.reset();
  }
}
