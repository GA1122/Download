UrlFetcher::UrlFetcher(const GURL& url, Method method)
    : core_ (new Core(url, method)) {
}
