OSExchangeData::DownloadFileInfo::DownloadFileInfo(
    const base::FilePath& filename,
    DownloadFileProvider* downloader)
    : filename(filename),
      downloader(downloader) {
}
