void RenderViewHostImpl::SetNavigationStartTime(
    const base::TimeTicks& navigation_start) {
  Send(new ViewMsg_SetNavigationStartTime(GetRoutingID(), navigation_start));
}
