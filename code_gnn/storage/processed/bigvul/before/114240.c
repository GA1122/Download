void CommandBufferProxyImpl::SetNotifyRepaintTask(const base::Closure& task) {
  notify_repaint_task_ = task;
}
