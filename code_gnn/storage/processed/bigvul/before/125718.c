void RenderViewHostImpl::SwapOut(int new_render_process_host_id,
                                 int new_request_id) {
  is_waiting_for_unload_ack_ = true;
  increment_in_flight_event_count();
  StartHangMonitorTimeout(TimeDelta::FromMilliseconds(kUnloadTimeoutMS));

  ViewMsg_SwapOut_Params params;
  params.closing_process_id = GetProcess()->GetID();
  params.closing_route_id = GetRoutingID();
  params.new_render_process_host_id = new_render_process_host_id;
  params.new_request_id = new_request_id;
  if (IsRenderViewLive()) {
    Send(new ViewMsg_SwapOut(GetRoutingID(), params));
  } else {
    GetProcess()->SimulateSwapOutACK(params);
  }
}
