  LoginHandlerAndroid(
      net::AuthChallengeInfo* auth_info,
      content::ResourceRequestInfo::WebContentsGetter web_contents_getter,
      LoginAuthRequiredCallback auth_required_callback)
      : LoginHandler(auth_info,
                     web_contents_getter,
                     std::move(auth_required_callback)) {}
