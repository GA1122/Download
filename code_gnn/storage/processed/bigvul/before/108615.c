  void DoAuthCallback(NetworkDelegate::AuthRequiredResponse response) {
    ASSERT_EQ(ON_AUTH_REQUIRED, state_);
    AuthCallback auth_callback = auth_callback_;
    Reset();
    auth_callback.Run(response);
  }
