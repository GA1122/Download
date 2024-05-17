  ExtensionRequestInterceptor()
      : interceptor_(
            base::BindRepeating(&ExtensionRequestInterceptor::OnRequest,
                                base::Unretained(this))) {}
