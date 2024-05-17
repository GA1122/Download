void CommandBufferProxyImpl::OnNotifyRepaint() {
  if (!notify_repaint_task_.is_null())
    MessageLoop::current()->PostNonNestableTask(
        FROM_HERE, notify_repaint_task_);
  notify_repaint_task_.Reset();
}
