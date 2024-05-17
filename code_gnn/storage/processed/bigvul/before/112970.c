bool ChromeDownloadManagerDelegate::ShouldOpenDownload(DownloadItem* item) {
  if (download_crx_util::IsExtensionDownload(*item)) {
    scoped_refptr<CrxInstaller> crx_installer =
        download_crx_util::OpenChromeExtension(profile_, *item);

    registrar_.Add(this,
                   chrome::NOTIFICATION_CRX_INSTALLER_DONE,
                   content::Source<CrxInstaller>(crx_installer.get()));

    crx_installers_[crx_installer.get()] = item->GetId();
    item->UpdateObservers();
    return false;
  }

  if (ShouldOpenWithWebIntents(item)) {
    OpenWithWebIntent(item);
    item->DelayedDownloadOpened(true  );
    return false;
  }

  return true;
}
