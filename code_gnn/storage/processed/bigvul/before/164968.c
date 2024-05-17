bool ResourceDispatcherHostImpl::ShouldServiceRequest(
    int child_id,
    const network::ResourceRequest& request_data,
    const net::HttpRequestHeaders& headers,
    ResourceRequesterInfo* requester_info,
    ResourceContext* resource_context) {
  ChildProcessSecurityPolicyImpl* policy =
      ChildProcessSecurityPolicyImpl::GetInstance();

  if (!policy->CanRequestURL(child_id, request_data.url)) {
    VLOG(1) << "Denied unauthorized request for "
            << request_data.url.possibly_invalid_spec();
    return false;
  }

  std::string origin_string;
  bool has_origin =
      headers.GetHeader("Origin", &origin_string) && origin_string != "null";
  if (has_origin) {
    GURL origin(origin_string);
    if (!policy->CanSetAsOriginHeader(child_id, origin)) {
      VLOG(1) << "Killed renderer for illegal origin: " << origin_string;
      bad_message::ReceivedBadMessage(requester_info->filter(),
                                      bad_message::RDH_ILLEGAL_ORIGIN);
      return false;
    }
  }

  if (!policy->CanReadRequestBody(child_id,
                                  requester_info->file_system_context(),
                                  request_data.request_body)) {
    NOTREACHED() << "Denied unauthorized upload";
    return false;
  }

  if (!ValidatePluginChildId(request_data.plugin_child_id)) {
    NOTREACHED() << "Invalid request_data.plugin_child_id: "
                 << request_data.plugin_child_id << " (" << child_id << ", "
                 << request_data.render_frame_id << ")";
    return false;
  }

  return true;
}
