TestURLFetcher::TestURLFetcher(int id,
                               const GURL& url,
                               URLFetcher::RequestType request_type,
                               URLFetcher::Delegate* d)
    : URLFetcher(url, request_type, d),
      id_(id),
      original_url_(url),
      did_receive_last_chunk_(false) {
}
