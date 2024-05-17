bool ChromeDownloadManagerDelegate::ShouldOpenDownload(
    DownloadItem* item, const content::DownloadOpenDelayedCallback& callback) {
#if BUILDFLAG(ENABLE_EXTENSIONS)
  if (download_crx_util::IsExtensionDownload(*item) &&
      !extensions::WebstoreInstaller::GetAssociatedApproval(*item)) {
    scoped_refptr<extensions::CrxInstaller> crx_installer =
        download_crx_util::OpenChromeExtension(profile_, *item);

    registrar_.Add(
        this,
        extensions::NOTIFICATION_CRX_INSTALLER_DONE,
        content::Source<extensions::CrxInstaller>(crx_installer.get()));

    crx_installers_[crx_installer.get()] = callback;
    item->UpdateObservers();
    return false;
  }
#endif

  return true;
}
