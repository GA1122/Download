scoped_refptr<LoginHandler> LoginHandler::Create(
    net::AuthChallengeInfo* auth_info,
    content::ResourceRequestInfo::WebContentsGetter web_contents_getter,
    LoginAuthRequiredCallback auth_required_callback) {
  return base::MakeRefCounted<LoginHandlerAndroid>(
      auth_info, web_contents_getter, std::move(auth_required_callback));
}
