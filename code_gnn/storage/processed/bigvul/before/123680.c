void GpuCommandBufferStub::OnSignalSyncPointAck(uint32 id) {
  Send(new GpuCommandBufferMsg_SignalSyncPointAck(route_id_, id));
}
