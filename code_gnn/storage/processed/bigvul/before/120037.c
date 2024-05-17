bool FileBrowserPrivateGetDriveFilesFunction::RunAsync() {
  using extensions::api::file_browser_private::GetDriveFiles::Params;
  const scoped_ptr<Params> params(Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params);

  for (size_t i = 0; i < params->file_urls.size(); ++i) {
    const base::FilePath path = file_manager::util::GetLocalPathFromURL(
        render_view_host(), GetProfile(), GURL(params->file_urls[i]));
    DCHECK(drive::util::IsUnderDriveMountPoint(path));
    base::FilePath drive_path = drive::util::ExtractDrivePath(path);
    remaining_drive_paths_.push(drive_path);
  }

  GetFileOrSendResponse();
  return true;
}
