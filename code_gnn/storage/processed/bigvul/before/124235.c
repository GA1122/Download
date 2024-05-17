int BuildLoadFlagsForRequest(const ResourceHostMsg_Request& request_data,
                             int child_id, bool is_sync_load) {
  int load_flags = request_data.load_flags;

  load_flags |= net::LOAD_VERIFY_EV_CERT;
  if (request_data.resource_type == ResourceType::MAIN_FRAME) {
    load_flags |= net::LOAD_MAIN_FRAME;
  } else if (request_data.resource_type == ResourceType::SUB_FRAME) {
    load_flags |= net::LOAD_SUB_FRAME;
  } else if (request_data.resource_type == ResourceType::PREFETCH) {
    load_flags |= (net::LOAD_PREFETCH | net::LOAD_DO_NOT_PROMPT_FOR_LOGIN);
  } else if (request_data.resource_type == ResourceType::FAVICON) {
    load_flags |= net::LOAD_DO_NOT_PROMPT_FOR_LOGIN;
  }

  if (is_sync_load)
    load_flags |= net::LOAD_IGNORE_LIMITS;

  ChildProcessSecurityPolicyImpl* policy =
      ChildProcessSecurityPolicyImpl::GetInstance();
  if (!policy->CanSendCookiesForOrigin(child_id, request_data.url)) {
    load_flags |= (net::LOAD_DO_NOT_SEND_COOKIES |
                   net::LOAD_DO_NOT_SEND_AUTH_DATA |
                   net::LOAD_DO_NOT_SAVE_COOKIES);
  }

  if ((load_flags & net::LOAD_REPORT_RAW_HEADERS)
      && !policy->CanReadRawCookies(child_id)) {
    VLOG(1) << "Denied unauthorized request for raw headers";
    load_flags &= ~net::LOAD_REPORT_RAW_HEADERS;
  }

  return load_flags;
}
