void CommandBufferProxyImpl::Flush(int32 put_offset) {
  if (last_state_.error != gpu::error::kNoError)
    return;

  TRACE_EVENT1("gpu",
               "CommandBufferProxyImpl::Flush",
               "put_offset",
               put_offset);

  if (last_put_offset_ == put_offset)
    return;

  last_put_offset_ = put_offset;

  Send(new GpuCommandBufferMsg_AsyncFlush(route_id_,
                                          put_offset,
                                          ++flush_count_));
}
