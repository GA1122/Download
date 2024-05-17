void Browser::OnStartDownload(TabContents* source, DownloadItem* download) {
  TabContentsWrapper* wrapper =
      TabContentsWrapper::GetCurrentWrapperForContents(source);
  TabContentsWrapper* constrained = GetConstrainingContentsWrapper(wrapper);
  if (constrained != wrapper) {
    TabContents* constrained_tab = constrained->tab_contents();
    constrained_tab->delegate()->OnStartDownload(constrained_tab, download);
    return;
  }

  if (!window())
    return;

  if (DisplayOldDownloadsUI()) {
#if defined(OS_CHROMEOS)
    ExtensionService* service = profile_->GetExtensionService();
    if (!ChromeDownloadManagerDelegate::IsExtensionDownload(download) ||
        (service == NULL) ||
        !service->IsDownloadFromGallery(download->GetURL(),
                                        download->referrer_url())) {
      ActiveDownloadsUI::OpenPopup(profile_);
    }
#else
    DownloadShelf* shelf = window()->GetDownloadShelf();
    shelf->AddDownload(new DownloadItemModel(download));
    TabContents* shelf_tab = shelf->browser()->GetSelectedTabContents();
    if ((download->total_bytes() > 0) &&
        (!ChromeDownloadManagerDelegate::IsExtensionDownload(download) ||
         ExtensionService::IsDownloadFromMiniGallery(download->GetURL())) &&
        platform_util::IsVisible(shelf_tab->GetNativeView()) &&
        ui::Animation::ShouldRenderRichAnimation()) {
      DownloadStartedAnimation::Show(shelf_tab);
    }
#endif
  }

  if (source->controller().IsInitialNavigation() && tab_count() > 1)
    CloseContents(source);
}
