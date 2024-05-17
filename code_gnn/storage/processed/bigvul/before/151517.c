  std::unique_ptr<WebWorkerFetchContext> TakeContext() {
    return std::move(web_context_);
  }
