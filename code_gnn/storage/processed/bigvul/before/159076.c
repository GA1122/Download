void ChromeDownloadManagerDelegate::SanitizeSavePackageResourceName(
    base::FilePath* filename) {
  safe_browsing::FileTypePolicies* file_type_policies =
      safe_browsing::FileTypePolicies::GetInstance();

  if (file_type_policies->GetFileDangerLevel(*filename) ==
      safe_browsing::DownloadFileType::NOT_DANGEROUS)
    return;

  base::FilePath default_filename = base::FilePath::FromUTF8Unsafe(
      l10n_util::GetStringUTF8(IDS_DEFAULT_DOWNLOAD_FILENAME));
  *filename = filename->AddExtension(default_filename.BaseName().value());
}
