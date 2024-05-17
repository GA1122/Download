bool TestURLFetcher::GetResponseAsFilePath(
    bool take_ownership, FilePath* out_response_path) const {
  if (GetResponseDestinationForTesting() != TEMP_FILE)
    return false;

  *out_response_path = fake_response_file_path_;
  return true;
}
