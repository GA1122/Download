void GpuCommandBufferStub::ScheduleDelayedWork(int64 delay) {
  if (HasMoreWork() && !delayed_work_scheduled_) {
    delayed_work_scheduled_ = true;
    MessageLoop::current()->PostDelayedTask(
        FROM_HERE,
        base::Bind(&GpuCommandBufferStub::PollWork,
                   AsWeakPtr()),
        base::TimeDelta::FromMilliseconds(delay));
  }
}
