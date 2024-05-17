void GpuMessageFilter::CreateCommandBufferCallback(
    IPC::Message* reply, int32 route_id) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));
  GpuHostMsg_CreateViewCommandBuffer::WriteReplyParams(reply, route_id);
  Send(reply);
}
