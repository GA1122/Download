  bool WaitForExtensionHostsToLoad() {

    NotificationRegistrar registrar;
    registrar.Add(this, NotificationType::EXTENSION_HOST_DID_STOP_LOADING,
                  NotificationService::AllSources());
    CancelableQuitTask* delayed_quit =
        new CancelableQuitTask("Extension host load timed out.");
    MessageLoop::current()->PostDelayedTask(FROM_HERE, delayed_quit,
        4*1000);

    ExtensionProcessManager* manager =
          browser()->profile()->GetExtensionProcessManager();
    for (ExtensionProcessManager::const_iterator iter = manager->begin();
         iter != manager->end();) {
      if ((*iter)->did_stop_loading())
        ++iter;
      else
        ui_test_utils::RunMessageLoop();
    }

    delayed_quit->cancel();
    return true;
  }
