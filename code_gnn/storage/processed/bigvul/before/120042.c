bool FileBrowserPrivateRequestDriveShareFunction::RunAsync() {
  using extensions::api::file_browser_private::RequestDriveShare::Params;
  const scoped_ptr<Params> params(Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params);

  const base::FilePath path = file_manager::util::GetLocalPathFromURL(
      render_view_host(), GetProfile(), GURL(params->url));
  const base::FilePath drive_path = drive::util::ExtractDrivePath(path);
  Profile* const owner_profile = drive::util::ExtractProfileFromPath(path);

  if (!owner_profile)
    return false;

  drive::FileSystemInterface* const owner_file_system =
      drive::util::GetFileSystemByProfile(owner_profile);
  if (!owner_file_system)
    return false;

  const user_manager::User* const user =
      chromeos::ProfileHelper::Get()->GetUserByProfile(GetProfile());
  if (!user || !user->is_logged_in())
    return false;

  google_apis::drive::PermissionRole role =
      google_apis::drive::PERMISSION_ROLE_READER;
  switch (params->share_type) {
    case api::file_browser_private::DRIVE_SHARE_TYPE_NONE:
      NOTREACHED();
      return false;
    case api::file_browser_private::DRIVE_SHARE_TYPE_CAN_EDIT:
      role = google_apis::drive::PERMISSION_ROLE_WRITER;
      break;
    case api::file_browser_private::DRIVE_SHARE_TYPE_CAN_COMMENT:
      role = google_apis::drive::PERMISSION_ROLE_COMMENTER;
      break;
    case api::file_browser_private::DRIVE_SHARE_TYPE_CAN_VIEW:
      role = google_apis::drive::PERMISSION_ROLE_READER;
      break;
  }

  owner_file_system->AddPermission(
      drive_path,
      user->email(),
      role,
      base::Bind(&FileBrowserPrivateRequestDriveShareFunction::OnAddPermission,
                 this));
  return true;
}
