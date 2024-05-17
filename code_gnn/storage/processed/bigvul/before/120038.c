bool FileBrowserPrivateCancelFileTransfersFunction::RunAsync() {
  using extensions::api::file_browser_private::CancelFileTransfers::Params;
  const scoped_ptr<Params> params(Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params);

  drive::DriveIntegrationService* integration_service =
      drive::DriveIntegrationServiceFactory::FindForProfile(GetProfile());
  if (!integration_service || !integration_service->IsMounted())
    return false;

  drive::JobListInterface* job_list = integration_service->job_list();
  DCHECK(job_list);
  std::vector<drive::JobInfo> jobs = job_list->GetJobInfoList();

  typedef std::map<base::FilePath, std::vector<drive::JobID> > PathToIdMap;
  PathToIdMap path_to_id_map;
  for (size_t i = 0; i < jobs.size(); ++i) {
    if (drive::IsActiveFileTransferJobInfo(jobs[i]))
      path_to_id_map[jobs[i].file_path].push_back(jobs[i].job_id);
  }

  std::vector<linked_ptr<api::file_browser_private::
                         FileTransferCancelStatus> > responses;
  for (size_t i = 0; i < params->file_urls.size(); ++i) {
    base::FilePath file_path = file_manager::util::GetLocalPathFromURL(
        render_view_host(), GetProfile(), GURL(params->file_urls[i]));
    if (file_path.empty())
      continue;

    DCHECK(drive::util::IsUnderDriveMountPoint(file_path));
    file_path = drive::util::ExtractDrivePath(file_path);

    PathToIdMap::iterator it = path_to_id_map.find(file_path);
    if (it != path_to_id_map.end()) {
      for (size_t i = 0; i < it->second.size(); ++i)
        job_list->CancelJob(it->second[i]);
    }
    linked_ptr<api::file_browser_private::FileTransferCancelStatus> result(
        new api::file_browser_private::FileTransferCancelStatus);
    result->canceled = it != path_to_id_map.end();
    result->file_url = params->file_urls[i];
    responses.push_back(result);
  }
  results_ = api::file_browser_private::CancelFileTransfers::Results::Create(
      responses);
  SendResponse(true);
  return true;
}
