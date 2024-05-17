void RedirectHostsToTestData(const char* const urls[], size_t size) {
  net::URLRequestFilter* filter = net::URLRequestFilter::GetInstance();
  base::FilePath base_path;
  GetTestDataDirectory(&base_path);
  for (size_t i = 0; i < size; ++i) {
    const GURL url(urls[i]);
    EXPECT_TRUE(url.is_valid());
    filter->AddUrlInterceptor(
        url, URLRequestMockHTTPJob::CreateInterceptor(base_path));
  }
}
