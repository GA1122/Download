  MHTMLBodyLoaderClient(
      std::unique_ptr<blink::WebNavigationParams> navigation_params,
      base::OnceCallback<void(std::unique_ptr<blink::WebNavigationParams>)>
          done_callback)
      : navigation_params_(std::move(navigation_params)),
        done_callback_(std::move(done_callback)) {
    body_loader_ = std::move(navigation_params_->body_loader);
    body_loader_->StartLoadingBody(this, false  );
  }
