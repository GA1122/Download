  TestUrlData(const GURL& url, CORSMode cors_mode, UrlIndex* url_index)
      : UrlData(url, cors_mode, url_index),
        block_shift_(url_index->block_shift()) {}
