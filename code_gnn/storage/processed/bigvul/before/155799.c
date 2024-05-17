  void AddSuccessfulOAuhTokenResponse() {
    client_->test_url_loader_factory()->AddResponse(
        GaiaUrls::GetInstance()->oauth2_token_url().spec(),
         GetValidTokenResponse("token", 3600));
   }
