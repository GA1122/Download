void ResourceDispatcherHost::OnAuthRequired(
    URLRequest* request,
    net::AuthChallengeInfo* auth_info) {
  ResourceDispatcherHostRequestInfo* info = InfoForRequest(request);
  DCHECK(!info->login_handler()) <<
      "OnAuthRequired called with login_handler pending";
  info->set_login_handler(CreateLoginPrompt(auth_info, request));
}
