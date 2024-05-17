void Browser::OnStartDownload(WebContents* source,
                              content::DownloadItem* download) {
  if (!download_util::ShouldShowInShelf(download))
    return;

  WebContents* constrained = GetConstrainingWebContents(source);
  if (constrained != source) {
    constrained->GetDelegate()->OnStartDownload(constrained, download);
    return;
  }

  if (!window())
    return;

  DownloadShelf* shelf = window()->GetDownloadShelf();
  shelf->AddDownload(new DownloadItemModel(download));
  WebContents* shelf_tab = chrome::GetActiveWebContents(shelf->browser());
  if ((download->GetTotalBytes() > 0) &&
      !download_crx_util::IsExtensionDownload(*download) &&
      platform_util::IsVisible(shelf_tab->GetNativeView()) &&
      ui::Animation::ShouldRenderRichAnimation()) {
    DownloadStartedAnimation::Show(shelf_tab);
  }

  if (source->GetController().IsInitialNavigation() && tab_count() > 1 &&
      !download->IsSavePackageDownload())
    CloseContents(source);
}
