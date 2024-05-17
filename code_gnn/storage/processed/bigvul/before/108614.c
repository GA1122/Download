  void DoAuthCallback(const AuthCallback& callback,
                      AuthCredentials* credentials) {
    if (auth_callback_retval_ ==
        NetworkDelegate::AUTH_REQUIRED_RESPONSE_SET_AUTH) {
      *credentials = auth_credentials_;
    }
    callback.Run(auth_callback_retval_);
  }
