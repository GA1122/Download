bool FileBrowserPrivatePinDriveFileFunction::RunAsync() {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  using extensions::api::file_browser_private::PinDriveFile::Params;
  const scoped_ptr<Params> params(Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params);

  drive::FileSystemInterface* const file_system =
      drive::util::GetFileSystemByProfile(GetProfile());
  if (!file_system)   
    return false;

  const base::FilePath drive_path =
      drive::util::ExtractDrivePath(file_manager::util::GetLocalPathFromURL(
          render_view_host(), GetProfile(), GURL(params->file_url)));
  if (params->pin) {
    file_system->Pin(drive_path,
                     base::Bind(&FileBrowserPrivatePinDriveFileFunction::
                                    OnPinStateSet, this));
  } else {
    file_system->Unpin(drive_path,
                       base::Bind(&FileBrowserPrivatePinDriveFileFunction::
                                      OnPinStateSet, this));
  }
  return true;
}
