void FileBrowserPrivateGetDownloadUrlFunction::OnTokenFetched(
    google_apis::GDataErrorCode code,
    const std::string& access_token) {
  if (code != google_apis::HTTP_SUCCESS) {
    SetError("Not able to fetch the token.");
    SetResult(new base::StringValue(""));   
    SendResponse(false);
    return;
  }

  const std::string url = download_url_ + "?access_token=" + access_token;
  SetResult(new base::StringValue(url));

  SendResponse(true);
}
