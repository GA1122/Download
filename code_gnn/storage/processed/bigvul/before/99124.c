bool ShouldServiceRequest(ChildProcessInfo::ProcessType process_type,
                          int child_id,
                          const ViewHostMsg_Resource_Request& request_data)  {
  if (process_type == ChildProcessInfo::PLUGIN_PROCESS)
    return true;

  ChildProcessSecurityPolicy* policy =
      ChildProcessSecurityPolicy::GetInstance();

  if (!policy->CanRequestURL(child_id, request_data.url)) {
    LOG(INFO) << "Denied unauthorized request for " <<
        request_data.url.possibly_invalid_spec();
    return false;
  }

  if (request_data.upload_data) {
    const std::vector<net::UploadData::Element>& uploads =
        request_data.upload_data->elements();
    std::vector<net::UploadData::Element>::const_iterator iter;
    for (iter = uploads.begin(); iter != uploads.end(); ++iter) {
      if (iter->type() == net::UploadData::TYPE_FILE &&
          !policy->CanUploadFile(child_id, iter->file_path())) {
        NOTREACHED() << "Denied unauthorized upload of "
                     << iter->file_path().value();
        return false;
      }
    }
  }

  return true;
}
