void FileBrowserPrivateRequestAccessTokenFunction::OnAccessTokenFetched(
    google_apis::GDataErrorCode code,
    const std::string& access_token) {
  SetResult(new base::StringValue(access_token));
  SendResponse(true);
}
