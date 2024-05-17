void ChromeDownloadManagerDelegate::Observe(
    int type,
    const content::NotificationSource& source,
    const content::NotificationDetails& details) {
#if BUILDFLAG(ENABLE_EXTENSIONS)
  DCHECK_EQ(extensions::NOTIFICATION_CRX_INSTALLER_DONE, type);

  registrar_.Remove(this, extensions::NOTIFICATION_CRX_INSTALLER_DONE, source);

  scoped_refptr<extensions::CrxInstaller> installer =
      content::Source<extensions::CrxInstaller>(source).ptr();
  content::DownloadOpenDelayedCallback callback =
      crx_installers_[installer.get()];
  crx_installers_.erase(installer.get());
  callback.Run(installer->did_handle_successfully());
#endif
}
