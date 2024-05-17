RenderProcessHostImpl::RenderProcessHostImpl(
    BrowserContext* browser_context,
    StoragePartitionImpl* storage_partition_impl,
    bool is_guest)
        : fast_shutdown_started_(false),
          deleting_soon_(false),
          pending_views_(0),
          visible_widgets_(0),
          backgrounded_(true),
          ALLOW_THIS_IN_INITIALIZER_LIST(cached_dibs_cleaner_(
                FROM_HERE, base::TimeDelta::FromSeconds(5),
                this, &RenderProcessHostImpl::ClearTransportDIBCache)),
          is_initialized_(false),
          id_(ChildProcessHostImpl::GenerateChildProcessUniqueId()),
          browser_context_(browser_context),
          storage_partition_impl_(storage_partition_impl),
          sudden_termination_allowed_(true),
          ignore_input_events_(false),
#if defined(OS_ANDROID)
          dummy_shutdown_event_(false, false),
#endif
          is_guest_(is_guest) {
  widget_helper_ = new RenderWidgetHelper();

  ChildProcessSecurityPolicyImpl::GetInstance()->Add(GetID());

  CHECK(!g_exited_main_message_loop);
  RegisterHost(GetID(), this);
  g_all_hosts.Get().set_check_on_null_data(true);
  mark_child_process_activity_time();
}
