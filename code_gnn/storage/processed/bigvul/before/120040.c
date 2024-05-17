bool FileBrowserPrivateSearchDriveMetadataFunction::RunAsync() {
  using api::file_browser_private::SearchDriveMetadata::Params;
  const scoped_ptr<Params> params(Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params);

  drive::EventLogger* logger = file_manager::util::GetLogger(GetProfile());
  if (logger) {
    logger->Log(logging::LOG_INFO,
                "%s[%d] called. (types: '%s', maxResults: '%d')",
                name().c_str(),
                request_id(),
                api::file_browser_private::ToString(
                    params->search_params.types).c_str(),
                params->search_params.max_results);
  }
  set_log_on_completion(true);

  drive::FileSystemInterface* const file_system =
      drive::util::GetFileSystemByProfile(GetProfile());
  if (!file_system) {
    return false;
  }

  int options = -1;
  switch (params->search_params.types) {
    case api::file_browser_private::SEARCH_TYPE_EXCLUDE_DIRECTORIES:
      options = drive::SEARCH_METADATA_EXCLUDE_DIRECTORIES;
      break;
    case api::file_browser_private::SEARCH_TYPE_SHARED_WITH_ME:
      options = drive::SEARCH_METADATA_SHARED_WITH_ME;
      break;
    case api::file_browser_private::SEARCH_TYPE_OFFLINE:
      options = drive::SEARCH_METADATA_OFFLINE;
      break;
    case api::file_browser_private::SEARCH_TYPE_ALL:
      options = drive::SEARCH_METADATA_ALL;
      break;
    case api::file_browser_private::SEARCH_TYPE_NONE:
      break;
  }
  DCHECK_NE(options, -1);

  file_system->SearchMetadata(
      params->search_params.query,
      options,
      params->search_params.max_results,
      base::Bind(&FileBrowserPrivateSearchDriveMetadataFunction::
                     OnSearchMetadata, this));
  return true;
}
