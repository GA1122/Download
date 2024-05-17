void SupervisedUserService::OnBlacklistDownloadDone(
    const base::FilePath& path,
    FileDownloader::Result result) {
  DCHECK(blacklist_state_ == BlacklistLoadState::LOAD_STARTED);
  if (FileDownloader::IsSuccess(result)) {
    LoadBlacklistFromFile(path);
  } else {
    LOG(WARNING) << "Blacklist download failed";
  }
  blacklist_downloader_.reset();
}
