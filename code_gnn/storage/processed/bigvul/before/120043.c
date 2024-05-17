bool FileBrowserPrivateGetDownloadUrlFunction::RunAsync() {
  base::CommandLine* command_line = base::CommandLine::ForCurrentProcess();
  if (!command_line->HasSwitch(
          chromeos::switches::kEnableVideoPlayerChromecastSupport)) {
    SetError("Cast support is disabled.");
    SetResult(new base::StringValue(""));   
    return false;
  }

  using extensions::api::file_browser_private::GetShareUrl::Params;
  const scoped_ptr<Params> params(Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params);

  drive::FileSystemInterface* const file_system =
      drive::util::GetFileSystemByProfile(GetProfile());
  if (!file_system) {
    SetError("Drive is disabled or not mounted.");
    SetResult(new base::StringValue(""));   
    return false;
  }

  const base::FilePath path = file_manager::util::GetLocalPathFromURL(
      render_view_host(), GetProfile(), GURL(params->url));
  DCHECK(drive::util::IsUnderDriveMountPoint(path));
  base::FilePath file_path = drive::util::ExtractDrivePath(path);

  file_system->GetResourceEntry(
      file_path,
      base::Bind(&FileBrowserPrivateGetDownloadUrlFunction::OnGetResourceEntry,
                 this));
  return true;
}
