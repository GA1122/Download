OAuthProviderInfo OAuthProviderInfo::GetDefault() {
  OAuthProviderInfo result;
  result.access_token_url = kDefaultOAuth2TokenUrl;
  result.user_info_url = kDefaultOAuth2UserInfoUrl;
  return result;
 }
