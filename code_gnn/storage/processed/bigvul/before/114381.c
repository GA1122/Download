void GpuCommandBufferStub::Destroy() {
  scheduler_.reset();

  while (!delayed_echos_.empty()) {
    delete delayed_echos_.front();
    delayed_echos_.pop_front();
  }

  if (decoder_.get())
    decoder_->MakeCurrent();
  FOR_EACH_OBSERVER(DestructionObserver,
                    destruction_observers_,
                    OnWillDestroyStub(this));

  if (decoder_.get()) {
    decoder_->Destroy();
    decoder_.reset();
  }

  command_buffer_.reset();

  context_ = NULL;
  surface_ = NULL;

  channel_->gpu_channel_manager()->gpu_memory_manager()->ScheduleManage();
}
