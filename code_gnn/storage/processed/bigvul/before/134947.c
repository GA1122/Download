std::unique_ptr<OAuth2MintTokenFlow> DriveFsHost::Delegate::CreateMintTokenFlow(
    OAuth2MintTokenFlow::Delegate* delegate,
    const std::string& client_id,
    const std::string& app_id,
    const std::vector<std::string>& scopes) {
  return std::make_unique<OAuth2MintTokenFlow>(
      delegate, OAuth2MintTokenFlow::Parameters{
                    app_id, client_id, scopes, kIdentityConsumerId,
                    OAuth2MintTokenFlow::MODE_MINT_TOKEN_FORCE});
}
