bool ShouldServiceRequest(int process_type,
                          int child_id,
                          const ResourceHostMsg_Request& request_data,
                          const net::HttpRequestHeaders& headers,
                          ResourceMessageFilter* filter,
                          ResourceContext* resource_context) {
  if (process_type == PROCESS_TYPE_PLUGIN)
    return true;

  ChildProcessSecurityPolicyImpl* policy =
      ChildProcessSecurityPolicyImpl::GetInstance();

  if (!policy->CanRequestURL(child_id, request_data.url)) {
    VLOG(1) << "Denied unauthorized request for "
            << request_data.url.possibly_invalid_spec();
    return false;
  }

  std::string origin_string;
  bool has_origin = headers.GetHeader("Origin", &origin_string) &&
                    origin_string != "null";
  if (has_origin) {
    GURL origin(origin_string);
    if (!policy->CanCommitURL(child_id, origin) ||
        GetContentClient()->browser()->IsIllegalOrigin(resource_context,
                                                       child_id, origin)) {
      VLOG(1) << "Killed renderer for illegal origin: " << origin_string;
      bad_message::ReceivedBadMessage(filter, bad_message::RDH_ILLEGAL_ORIGIN);
      return false;
    }
  }

  if (request_data.request_body.get()) {
    const std::vector<ResourceRequestBody::Element>* uploads =
        request_data.request_body->elements();
    std::vector<ResourceRequestBody::Element>::const_iterator iter;
    for (iter = uploads->begin(); iter != uploads->end(); ++iter) {
      if (iter->type() == ResourceRequestBody::Element::TYPE_FILE &&
          !policy->CanReadFile(child_id, iter->path())) {
        NOTREACHED() << "Denied unauthorized upload of "
                     << iter->path().value();
        return false;
      }
      if (iter->type() == ResourceRequestBody::Element::TYPE_FILE_FILESYSTEM) {
        storage::FileSystemURL url =
            filter->file_system_context()->CrackURL(iter->filesystem_url());
        if (!policy->CanReadFileSystemFile(child_id, url)) {
          NOTREACHED() << "Denied unauthorized upload of "
                       << iter->filesystem_url().spec();
          return false;
        }
      }
    }
  }

  return true;
}
