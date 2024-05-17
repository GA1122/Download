  SyncManagerTest()
      : ui_thread_(BrowserThread::UI, &ui_loop_),
        sync_notifier_mock_(NULL),
        sync_manager_("Test sync manager"),
        sync_notifier_observer_(NULL),
        update_enabled_types_call_count_(0) {}
