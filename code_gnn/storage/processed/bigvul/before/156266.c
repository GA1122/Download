void RendererSchedulerImpl::SetTopLevelBlameContext(
    base::trace_event::BlameContext* blame_context) {
  control_task_queue_->SetBlameContext(blame_context);
  DefaultTaskQueue()->SetBlameContext(blame_context);
  default_timer_task_queue_->SetBlameContext(blame_context);
  compositor_task_queue_->SetBlameContext(blame_context);
  idle_helper_.IdleTaskRunner()->SetBlameContext(blame_context);
  v8_task_queue_->SetBlameContext(blame_context);
  ipc_task_queue_->SetBlameContext(blame_context);
}
