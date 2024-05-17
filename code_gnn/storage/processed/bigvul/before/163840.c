const Extension* ExtensionBrowserTest::LoadAndLaunchApp(
    const base::FilePath& path) {
  const Extension* app = LoadExtension(path);
  CHECK(app);
  content::WindowedNotificationObserver app_loaded_observer(
      content::NOTIFICATION_LOAD_COMPLETED_MAIN_FRAME,
      content::NotificationService::AllSources());
  AppLaunchParams params(profile(), app, extensions::LAUNCH_CONTAINER_NONE,
                         WindowOpenDisposition::NEW_WINDOW,
                         extensions::SOURCE_TEST);
  params.command_line = *base::CommandLine::ForCurrentProcess();
  OpenApplication(params);
  app_loaded_observer.Wait();

  return app;
}
