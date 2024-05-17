  std::unique_ptr<WebAssociatedURLLoader> CreateAssociatedURLLoader(
      const WebAssociatedURLLoaderOptions options =
          WebAssociatedURLLoaderOptions()) {
    return base::WrapUnique(MainFrame()->CreateAssociatedURLLoader(options));
  }
