void GpuChannel::PollWork(int route_id) {
  GpuCommandBufferStub* stub = stubs_.Lookup(route_id);
  if (stub) {
    stub->PollWork();

    ScheduleDelayedWork(stub, kHandleMoreWorkPeriodBusyMs);
  }
}
