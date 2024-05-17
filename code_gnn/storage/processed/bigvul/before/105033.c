  void RunDelegate() {
    net::URLRequestStatus status;
    status.set_status(success_ ? net::URLRequestStatus::SUCCESS :
                                 net::URLRequestStatus::FAILED);
    delegate()->OnURLFetchComplete(this, url_, status, success_ ? 200 : 500,
                                   net::ResponseCookies(), response_data_);
  }
