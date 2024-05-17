bool FileBrowserPrivateSearchDriveFunction::RunAsync() {
  using extensions::api::file_browser_private::SearchDrive::Params;
  const scoped_ptr<Params> params(Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params);

  drive::FileSystemInterface* const file_system =
      drive::util::GetFileSystemByProfile(GetProfile());
  if (!file_system) {
    return false;
  }

  file_system->Search(
      params->search_params.query, GURL(params->search_params.next_feed),
      base::Bind(&FileBrowserPrivateSearchDriveFunction::OnSearch, this));
  return true;
}
