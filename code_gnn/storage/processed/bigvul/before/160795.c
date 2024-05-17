void RenderViewImpl::NavigateBackForwardSoon(int offset) {
  history_navigation_virtual_time_pauser_ =
      RenderThreadImpl::current()
          ->GetRendererScheduler()
          ->CreateWebScopedVirtualTimePauser();
  history_navigation_virtual_time_pauser_.PauseVirtualTime(true);
  Send(new ViewHostMsg_GoToEntryAtOffset(GetRoutingID(), offset));
}
