void Browser::OnStartDownload(DownloadItem* download, TabContents* tab) {
  if (!window())
    return;

#if defined(OS_CHROMEOS)
  if (download->is_extension_install()) {
    ExtensionService* service = profile_->GetExtensionService();
    if (service && service->IsDownloadFromGallery(download->url(),
                                                  download->referrer_url())) {
      return;
    }
  }
  ActiveDownloadsUI::OpenPopup(profile_, download);
#else
  window()->GetDownloadShelf()->AddDownload(new DownloadItemModel(download));

  if (download->total_bytes() <= 0)
    return;

  if (download->is_extension_install() &&
      !ExtensionService::IsDownloadFromMiniGallery(download->url()))
    return;

  TabContents* current_tab = GetSelectedTabContents();
  if (platform_util::IsVisible(current_tab->GetNativeView()) &&
      ui::Animation::ShouldRenderRichAnimation()) {
    DownloadStartedAnimation::Show(current_tab);
  }
#endif

  TabContentsWrapper* wrapper =
      TabContentsWrapper::GetCurrentWrapperForContents(tab);
  if (tab->controller().IsInitialNavigation() &&
      GetConstrainingContentsWrapper(wrapper) == wrapper && tab_count() > 1) {
    CloseContents(tab);
  }
}
