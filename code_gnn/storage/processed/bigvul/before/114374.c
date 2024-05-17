void GpuChannel::ScheduleDelayedWork(GpuCommandBufferStub *stub,
                                     int64 delay) {
  if (stub->HasMoreWork()) {
    MessageLoop::current()->PostDelayedTask(
        FROM_HERE,
        base::Bind(&GpuChannel::PollWork,
                   weak_factory_.GetWeakPtr(),
                   stub->route_id()),
        base::TimeDelta::FromMilliseconds(delay));
  }
}
