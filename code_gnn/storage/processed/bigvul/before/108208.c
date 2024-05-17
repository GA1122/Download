  bool LoadExtensionFromPath(const FilePath& path) {
    ExtensionsService* service = browser()->profile()->GetExtensionsService();
    size_t num_before = service->extensions()->size();
    {
      NotificationRegistrar registrar;
      registrar.Add(this, NotificationType::EXTENSION_LOADED,
                    NotificationService::AllSources());
      CancelableQuitTask* delayed_quit =
          new CancelableQuitTask("Extension load timed out.");
      MessageLoop::current()->PostDelayedTask(FROM_HERE, delayed_quit,
          4*1000);
      service->LoadExtension(path);
      ui_test_utils::RunMessageLoop();
      delayed_quit->cancel();
    }
    size_t num_after = service->extensions()->size();
    if (num_after != (num_before + 1))
      return false;

    return WaitForExtensionHostsToLoad();
  }
