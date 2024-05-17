  void MaybeStartLoader(
      NavigationLoaderInterceptor* interceptor,
      SingleRequestURLLoaderFactory::RequestHandler single_request_handler) {
    DCHECK_CURRENTLY_ON(BrowserThread::IO);
    DCHECK(IsLoaderInterceptionEnabled());
    DCHECK(started_);

    if (single_request_handler) {
      DCHECK(interceptor);

      std::vector<std::unique_ptr<URLLoaderThrottle>> throttles =
          CreateURLLoaderThrottles();
      throttles.push_back(std::make_unique<MimeSniffingThrottle>());

      default_loader_used_ = false;
      url_loader_ = ThrottlingURLLoader::CreateLoaderAndStart(
          base::MakeRefCounted<SingleRequestURLLoaderFactory>(
              std::move(single_request_handler)),
          std::move(throttles), frame_tree_node_id_,
          global_request_id_.request_id, network::mojom::kURLLoadOptionNone,
          resource_request_.get(), this, kNavigationUrlLoaderTrafficAnnotation,
          base::ThreadTaskRunnerHandle::Get());

      subresource_loader_params_ =
          interceptor->MaybeCreateSubresourceLoaderParams();

      return;
    }

    if (interceptor) {
      subresource_loader_params_ =
          interceptor->MaybeCreateSubresourceLoaderParams();

      if (subresource_loader_params_)
        interceptor_index_ = interceptors_.size();
    }

    if (interceptor_index_ < interceptors_.size()) {
      auto* next_interceptor = interceptors_[interceptor_index_++].get();
      next_interceptor->MaybeCreateLoader(
          *resource_request_, resource_context_,
          base::BindOnce(&URLLoaderRequestController::MaybeStartLoader,
                         base::Unretained(this), next_interceptor),
          base::BindOnce(
              &URLLoaderRequestController::FallbackToNonInterceptedRequest,
              base::Unretained(this)));
      return;
    }

    if (url_loader_) {
      DCHECK(!redirect_info_.new_url.is_empty());
      url_loader_->FollowRedirect(
          std::move(url_loader_modified_request_headers_));
      return;
    }

    uint32_t options = network::mojom::kURLLoadOptionNone;
    scoped_refptr<network::SharedURLLoaderFactory> factory =
        PrepareForNonInterceptedRequest(&options);
    url_loader_ = ThrottlingURLLoader::CreateLoaderAndStart(
        std::move(factory), CreateURLLoaderThrottles(), frame_tree_node_id_,
        global_request_id_.request_id, options, resource_request_.get(),
        this  , kNavigationUrlLoaderTrafficAnnotation,
        base::ThreadTaskRunnerHandle::Get());
  }
