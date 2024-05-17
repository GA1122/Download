void FetchManager::Loader::DidFailRedirectCheck() {
  Failed("Fetch API cannot load " + fetch_request_data_->Url().GetString() +
         ". Redirect failed.");
}
