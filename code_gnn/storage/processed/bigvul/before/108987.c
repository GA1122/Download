void RenderViewImpl::OnShouldClose() {
  base::TimeTicks before_unload_start_time = base::TimeTicks::Now();
  bool should_close = webview()->dispatchBeforeUnloadEvent();
  base::TimeTicks before_unload_end_time = base::TimeTicks::Now();
  Send(new ViewHostMsg_ShouldClose_ACK(routing_id_, should_close,
                                       before_unload_start_time,
                                       before_unload_end_time));
}
