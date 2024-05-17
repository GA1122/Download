bool FileBrowserPrivateGetDriveEntryPropertiesFunction::RunAsync() {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  using api::file_browser_private::GetDriveEntryProperties::Params;
  const scoped_ptr<Params> params(Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params);

  properties_list_.resize(params->file_urls.size());

  for (size_t i = 0; i < params->file_urls.size(); i++) {
    const GURL url = GURL(params->file_urls[i]);
    const base::FilePath local_path = file_manager::util::GetLocalPathFromURL(
        render_view_host(), GetProfile(), url);
    properties_list_[i] = make_linked_ptr(new DriveEntryProperties);

    SingleDriveEntryPropertiesGetter::Start(
        local_path,
        properties_list_[i],
        GetProfile(),
        base::Bind(&FileBrowserPrivateGetDriveEntryPropertiesFunction::
                       CompleteGetFileProperties,
                   this));
  }

  return true;
}
