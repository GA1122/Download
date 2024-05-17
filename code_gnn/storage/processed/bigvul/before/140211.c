void WebResourceService::OnURLFetchComplete(const net::URLFetcher* source) {
  std::unique_ptr<net::URLFetcher> clean_up_fetcher(url_fetcher_.release());

  if (source->GetStatus().is_success() && source->GetResponseCode() == 200) {
    std::string data;
    source->GetResponseAsString(&data);
    if (data.empty() || data == "{}") {
      OnUnpackFinished(base::MakeUnique<base::DictionaryValue>());
    } else {
      parse_json_callback_.Run(data,
                               base::Bind(&WebResourceService::OnUnpackFinished,
                                          weak_ptr_factory_.GetWeakPtr()),
                               base::Bind(&WebResourceService::OnUnpackError,
                                          weak_ptr_factory_.GetWeakPtr()));
    }
  } else {
    EndFetch();
  }
}
