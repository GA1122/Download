  virtual void OnURLFetchComplete(const net::URLFetcher* source) {
    std::string data;
    source->GetResponseAsString(&data);
    running_ =
        (source->GetStatus().status() == net::URLRequestStatus::SUCCESS &&
        source->GetResponseCode() == 200 && data.find("ok") == 0);
    MessageLoop::current()->Quit();
  }
