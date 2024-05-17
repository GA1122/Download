base::FilePath DownloadManagerImpl::GetDefaultDownloadDirectory() {
  base::FilePath default_download_directory;
#if defined(USE_X11)
  default_download_directory = GetTemporaryDownloadDirectory();
#else
  if (delegate_) {
    base::FilePath website_save_directory;   
    bool skip_dir_check = false;             
    delegate_->GetSaveDir(GetBrowserContext(), &website_save_directory,
                          &default_download_directory, &skip_dir_check);
  }
#endif
  if (default_download_directory.empty()) {
    default_download_directory =
        GetContentClient()->browser()->GetDefaultDownloadDirectory();
  }

  return default_download_directory;
}
