   MutableProfileOAuth2TokenServiceDelegateTest()
      : signin_error_controller_(
            SigninErrorController::AccountMode::ANY_ACCOUNT),
        access_token_success_count_(0),
        access_token_failure_count_(0),
        access_token_failure_(GoogleServiceAuthError::NONE),
        token_available_count_(0),
        token_revoked_count_(0),
        tokens_loaded_count_(0),
        start_batch_changes_(0),
        end_batch_changes_(0),
        auth_error_changed_count_(0),
        revoke_all_tokens_on_load_(false) {}
