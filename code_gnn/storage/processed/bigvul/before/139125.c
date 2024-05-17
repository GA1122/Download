void RenderProcessHostImpl::Cleanup() {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  if (run_renderer_in_process())
    return;

  if (within_process_died_observer_) {
    delayed_cleanup_needed_ = true;
    return;
  }
  delayed_cleanup_needed_ = false;

  if (listeners_.IsEmpty() && GetWorkerRefCount() > 0 &&
      survive_for_worker_start_time_.is_null()) {
    survive_for_worker_start_time_ = base::TimeTicks::Now();
  }

  if (!listeners_.IsEmpty() || GetWorkerRefCount() != 0)
    return;

#if BUILDFLAG(ENABLE_WEBRTC)
  if (is_initialized_)
    ClearWebRtcLogMessageCallback();
#endif

  if (!survive_for_worker_start_time_.is_null()) {
    UMA_HISTOGRAM_LONG_TIMES(
        "SharedWorker.RendererSurviveForWorkerTime",
        base::TimeTicks::Now() - survive_for_worker_start_time_);
  }

  DCHECK(!deleting_soon_);

  DCHECK_EQ(0, pending_views_);

  if (HasConnection()) {
    for (auto& observer : observers_) {
      observer.RenderProcessExited(
          this, base::TERMINATION_STATUS_NORMAL_TERMINATION, 0);
    }
  }
  for (auto& observer : observers_)
    observer.RenderProcessHostDestroyed(this);
  NotificationService::current()->Notify(
      NOTIFICATION_RENDERER_PROCESS_TERMINATED,
      Source<RenderProcessHost>(this), NotificationService::NoDetails());

  if (connection_filter_id_ !=
        ServiceManagerConnection::kInvalidConnectionFilterId) {
    ServiceManagerConnection* service_manager_connection =
        BrowserContext::GetServiceManagerConnectionFor(browser_context_);
    connection_filter_controller_->DisableFilter();
    service_manager_connection->RemoveConnectionFilter(connection_filter_id_);
    connection_filter_id_ =
        ServiceManagerConnection::kInvalidConnectionFilterId;
  }

#ifndef NDEBUG
  is_self_deleted_ = true;
#endif
  base::ThreadTaskRunnerHandle::Get()->DeleteSoon(FROM_HERE, this);
  deleting_soon_ = true;

  ResetChannelProxy();

  message_port_message_filter_ = NULL;

  DCHECK(!channel_);
  RemoveUserData(kSessionStorageHolderKey);

  UnregisterHost(GetID());

  instance_weak_factory_.reset(
      new base::WeakPtrFactory<RenderProcessHostImpl>(this));
}
