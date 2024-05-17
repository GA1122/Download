void WebMediaPlayerImpl::ActivateViewportIntersectionMonitoring(bool activate) {
  DCHECK(main_task_runner_->BelongsToCurrentThread());

  client_->ActivateViewportIntersectionMonitoring(activate);
}
