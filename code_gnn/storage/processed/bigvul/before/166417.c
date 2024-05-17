std::string CreateHistogramNameWithSuffix(const std::string& name,
                                          DownloadSource download_source) {
  std::string suffix;
  switch (download_source) {
    case DownloadSource::UNKNOWN:
      suffix = "UnknownSource";
      break;
    case DownloadSource::NAVIGATION:
      suffix = "Navigation";
      break;
    case DownloadSource::DRAG_AND_DROP:
      suffix = "DragAndDrop";
      break;
    case DownloadSource::FROM_RENDERER:
      suffix = "FromRenderer";
      break;
    case DownloadSource::EXTENSION_API:
      suffix = "ExtensionAPI";
      break;
    case DownloadSource::EXTENSION_INSTALLER:
      suffix = "ExtensionInstaller";
      break;
    case DownloadSource::INTERNAL_API:
      suffix = "InternalAPI";
      break;
    case DownloadSource::WEB_CONTENTS_API:
      suffix = "WebContentsAPI";
      break;
    case DownloadSource::OFFLINE_PAGE:
      suffix = "OfflinePage";
      break;
    case DownloadSource::CONTEXT_MENU:
      suffix = "ContextMenu";
      break;
  }

  return name + "." + suffix;
}
