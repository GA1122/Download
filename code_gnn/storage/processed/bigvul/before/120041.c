bool FileBrowserPrivateRequestAccessTokenFunction::RunAsync() {
  using extensions::api::file_browser_private::RequestAccessToken::Params;
  const scoped_ptr<Params> params(Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params);

  drive::DriveServiceInterface* const drive_service =
      drive::util::GetDriveServiceByProfile(GetProfile());

  if (!drive_service) {
    SetResult(new base::StringValue(""));
    SendResponse(true);
    return true;
  }

  if (params->refresh)
    drive_service->ClearAccessToken();

  drive_service->RequestAccessToken(
      base::Bind(&FileBrowserPrivateRequestAccessTokenFunction::
                      OnAccessTokenFetched, this));
  return true;
}
