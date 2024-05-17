ResourceDispatcherHostImpl::ResourceDispatcherHostImpl(
    CreateDownloadHandlerIntercept download_handler_intercept,
    const scoped_refptr<base::SingleThreadTaskRunner>& io_thread_runner,
    bool enable_resource_scheduler)
    : request_id_(-1),
      is_shutdown_(false),
      enable_resource_scheduler_(enable_resource_scheduler),
      num_in_flight_requests_(0),
      max_num_in_flight_requests_(base::SharedMemory::GetHandleLimit()),
      max_num_in_flight_requests_per_process_(static_cast<int>(
          max_num_in_flight_requests_ * kMaxRequestsPerProcessRatio)),
      max_outstanding_requests_cost_per_process_(
          kMaxOutstandingRequestsCostPerProcess),
      delegate_(nullptr),
      loader_delegate_(nullptr),
      allow_cross_origin_auth_prompt_(false),
      create_download_handler_intercept_(download_handler_intercept),
      main_thread_task_runner_(base::ThreadTaskRunnerHandle::Get()),
      io_thread_task_runner_(io_thread_runner),
      weak_factory_on_io_(this) {
  DCHECK(main_thread_task_runner_->BelongsToCurrentThread());
  DCHECK(!g_resource_dispatcher_host);
  g_resource_dispatcher_host = this;

  ANNOTATE_BENIGN_RACE(
      &last_user_gesture_time_,
      "We don't care about the precise value, see http://crbug.com/92889");

  io_thread_task_runner_->PostTask(
      FROM_HERE, base::BindOnce(&ResourceDispatcherHostImpl::OnInit,
                                base::Unretained(this)));

  update_load_info_timer_ = std::make_unique<base::OneShotTimer>();
}
