void GpuCommandBufferStub::SetPreemptByCounter(
    scoped_refptr<gpu::RefCountedCounter> counter) {
  preempt_by_counter_ = counter;
  if (scheduler_.get())
    scheduler_->SetPreemptByCounter(preempt_by_counter_);
}
