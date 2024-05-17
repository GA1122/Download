void FileBrowserPrivateGetDownloadUrlFunction::OnGetResourceEntry(
    drive::FileError error,
    scoped_ptr<drive::ResourceEntry> entry) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  if (error != drive::FILE_ERROR_OK) {
    SetError("Download Url for this item is not available.");
    SetResult(new base::StringValue(""));   
    SendResponse(false);
    return;
  }

  download_url_ =
      google_apis::DriveApiUrlGenerator::kBaseDownloadUrlForProduction +
      entry->resource_id();

  ProfileOAuth2TokenService* oauth2_token_service =
      ProfileOAuth2TokenServiceFactory::GetForProfile(GetProfile());
  SigninManagerBase* signin_manager =
      SigninManagerFactory::GetForProfile(GetProfile());
  const std::string& account_id = signin_manager->GetAuthenticatedAccountId();
  std::vector<std::string> scopes;
  scopes.push_back("https://www.googleapis.com/auth/drive.readonly");

  auth_service_.reset(
      new google_apis::AuthService(oauth2_token_service,
                                   account_id,
                                   GetProfile()->GetRequestContext(),
                                   scopes));
  auth_service_->StartAuthentication(base::Bind(
      &FileBrowserPrivateGetDownloadUrlFunction::OnTokenFetched, this));
}
