   void CreateOAuth2ServiceDelegate(
//   std::unique_ptr<MutableProfileOAuth2TokenServiceDelegate>
//   CreateOAuth2ServiceDelegate(
        signin::AccountConsistencyMethod account_consistency) {
    oauth2_service_delegate_.reset(new MutableProfileOAuth2TokenServiceDelegate(
//     return std::make_unique<MutableProfileOAuth2TokenServiceDelegate>(
          client_.get(), &signin_error_controller_, &account_tracker_service_,
          token_web_data_, account_consistency, revoke_all_tokens_on_load_,
        true  ));
//         true  );
//      
      base::RunLoop().RunUntilIdle();
//   }
// 
//   void InitializeOAuth2ServiceDelegate(
//       signin::AccountConsistencyMethod account_consistency) {
//     oauth2_service_delegate_ = CreateOAuth2ServiceDelegate(account_consistency);
      oauth2_service_delegate_->AddObserver(this);
    }