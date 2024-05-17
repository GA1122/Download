void ChromeDownloadManagerDelegate::ChooseSavePath(
    WebContents* web_contents,
    const FilePath& suggested_path,
    const FilePath::StringType& default_extension,
    bool can_save_as_complete,
    const content::SavePackagePathPickedCallback& callback) {
#if defined(OS_CHROMEOS)
  new SavePackageFilePickerChromeOS(web_contents, suggested_path, callback);
#else
  new SavePackageFilePicker(web_contents, suggested_path, default_extension,
      can_save_as_complete, download_prefs_.get(), callback);
#endif
}
