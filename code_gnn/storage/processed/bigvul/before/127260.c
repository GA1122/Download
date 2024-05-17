void WtsSessionProcessDelegate::Core::OnIOCompleted(
    base::MessagePumpForIO::IOContext* context,
    DWORD bytes_transferred,
    DWORD error) {
  DCHECK(io_task_runner_->BelongsToCurrentThread());

  main_task_runner_->PostTask(FROM_HERE, base::Bind(
      &Core::OnJobNotification, this, bytes_transferred,
      reinterpret_cast<DWORD>(context)));
}
