void ContextMenuFilter::Wait() {
  if (!handled_)
    message_loop_runner_->Run();
}
