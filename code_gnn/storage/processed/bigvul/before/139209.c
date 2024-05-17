RenderProcessHostImpl::RenderProcessHostImpl(
    BrowserContext* browser_context,
    StoragePartitionImpl* storage_partition_impl,
    bool is_for_guests_only)
    : fast_shutdown_started_(false),
      deleting_soon_(false),
#ifndef NDEBUG
      is_self_deleted_(false),
#endif
      pending_views_(0),
      service_worker_ref_count_(0),
      shared_worker_ref_count_(0),
      is_worker_ref_count_disabled_(false),
      route_provider_binding_(this),
      visible_widgets_(0),
      is_process_backgrounded_(false),
      id_(ChildProcessHostImpl::GenerateChildProcessUniqueId()),
      browser_context_(browser_context),
      storage_partition_impl_(storage_partition_impl),
      sudden_termination_allowed_(true),
      ignore_input_events_(false),
      is_for_guests_only_(is_for_guests_only),
      gpu_observer_registered_(false),
      delayed_cleanup_needed_(false),
      within_process_died_observer_(false),
#if BUILDFLAG(ENABLE_WEBRTC)
      webrtc_eventlog_host_(id_),
#endif
      permission_service_context_(new PermissionServiceContext(this)),
      indexed_db_factory_(new IndexedDBDispatcherHost(
          id_,
          storage_partition_impl_->GetURLRequestContext(),
          storage_partition_impl_->GetIndexedDBContext(),
          ChromeBlobStorageContext::GetFor(browser_context_))),
      channel_connected_(false),
      sent_render_process_ready_(false),
#if defined(OS_ANDROID)
      never_signaled_(base::WaitableEvent::ResetPolicy::MANUAL,
                      base::WaitableEvent::InitialState::NOT_SIGNALED),
#endif
      instance_weak_factory_(
          new base::WeakPtrFactory<RenderProcessHostImpl>(this)),
      weak_factory_(this) {
  widget_helper_ = new RenderWidgetHelper();

  ChildProcessSecurityPolicyImpl::GetInstance()->Add(GetID());

  CHECK(!BrowserMainRunner::ExitedMainMessageLoop());
  RegisterHost(GetID(), this);
  g_all_hosts.Get().set_check_on_null_data(true);
  mark_child_process_activity_time();

  if (!GetBrowserContext()->IsOffTheRecord() &&
      !base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kDisableGpuShaderDiskCache)) {
    BrowserThread::PostTask(BrowserThread::IO, FROM_HERE,
                            base::Bind(&CacheShaderInfo, GetID(),
                                       storage_partition_impl_->GetPath()));
  }

#if defined(OS_MACOSX)
  if (BootstrapSandboxManager::ShouldEnable())
    AddObserver(BootstrapSandboxManager::GetInstance());
#endif

  InitializeChannelProxy();
}
