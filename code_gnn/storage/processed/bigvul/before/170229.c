  const extensions::Extension* InstallBookmarkApp() {
    WebApplicationInfo web_app;
    web_app.title = base::ASCIIToUTF16("Bookmark App");
    web_app.app_url = GURL("http://www.google.com");

    scoped_refptr<extensions::CrxInstaller> installer =
        extensions::CrxInstaller::CreateSilent(extension_service());

    content::WindowedNotificationObserver observer(
        extensions::NOTIFICATION_CRX_INSTALLER_DONE,
        content::NotificationService::AllSources());
    installer->InstallWebApp(web_app);
    observer.Wait();
    content::Details<const extensions::Extension> details = observer.details();
    return details.ptr();
  }
