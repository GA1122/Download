void GpuCommandBufferStub::OnAsyncFlush(int32 put_offset,
                                        uint32 flush_count) {
  TRACE_EVENT1("gpu", "GpuCommandBufferStub::OnAsyncFlush",
               "put_offset", put_offset);
  DCHECK(command_buffer_.get());
  if (flush_count - last_flush_count_ < 0x8000000U) {
    last_flush_count_ = flush_count;
    command_buffer_->Flush(put_offset);
  } else {
    NOTREACHED() << "Received a Flush message out-of-order";
  }

  ReportState();
}
