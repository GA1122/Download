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
      keep_alive_ref_count_(0),
      is_keep_alive_ref_count_disabled_(false),
      route_provider_binding_(this),
      visible_widgets_(0),
      priority_({
            kLaunchingProcessIsBackgrounded,
            kLaunchingProcessIsBoostedForPendingView,
#if defined(OS_ANDROID)
            ChildProcessImportance::NORMAL,
#endif
      }),
