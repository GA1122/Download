  PrepareForNonInterceptedRequest(uint32_t* out_options) {
    if (!base::FeatureList::IsEnabled(network::features::kNetworkService)) {
      DCHECK(!interceptors_.empty());
      DCHECK(default_request_handler_factory_);
      DCHECK(blink::ServiceWorkerUtils::IsServicificationEnabled() ||
             signed_exchange_utils::IsSignedExchangeHandlingEnabled());
      default_loader_used_ = true;
      if (url_chain_.size() > 0) {
        request_info_ = CreateNavigationRequestInfoForRedirect(
            *request_info_, *resource_request_);
      }

      bool was_request_intercepted = subresource_loader_params_.has_value();

      *out_options = network::mojom::kURLLoadOptionNone;
      return base::MakeRefCounted<SingleRequestURLLoaderFactory>(
          default_request_handler_factory_.Run(was_request_intercepted));
    }

    scoped_refptr<network::SharedURLLoaderFactory> factory;

    if (!IsURLHandledByDefaultLoader(resource_request_->url)) {
      if (known_schemes_.find(resource_request_->url.scheme()) ==
          known_schemes_.end()) {
        bool handled = GetContentClient()->browser()->HandleExternalProtocol(
            resource_request_->url, web_contents_getter_,
            ChildProcessHost::kInvalidUniqueID, navigation_ui_data_.get(),
            resource_request_->resource_type == RESOURCE_TYPE_MAIN_FRAME,
            static_cast<ui::PageTransition>(resource_request_->transition_type),
            resource_request_->has_user_gesture);
        factory = base::MakeRefCounted<SingleRequestURLLoaderFactory>(
            base::BindOnce(UnknownSchemeCallback, handled));
      } else {
        network::mojom::URLLoaderFactoryPtr& non_network_factory =
            non_network_url_loader_factories_[resource_request_->url.scheme()];
        if (!non_network_factory.is_bound()) {
          base::PostTaskWithTraits(
              FROM_HERE, {BrowserThread::UI},
              base::BindOnce(&NavigationURLLoaderImpl ::
                                 BindNonNetworkURLLoaderFactoryRequest,
                             owner_, frame_tree_node_id_,
                             resource_request_->url,
                             mojo::MakeRequest(&non_network_factory)));
        }
        factory =
            base::MakeRefCounted<network::WeakWrapperSharedURLLoaderFactory>(
                non_network_factory.get());
      }
    } else {
      default_loader_used_ = true;

      if (proxied_factory_request_.is_pending() &&
          !resource_request_->url.SchemeIs(url::kDataScheme)) {
        DCHECK(proxied_factory_info_.is_valid());
        network_loader_factory_->Clone(std::move(proxied_factory_request_));
        factory = base::MakeRefCounted<network::WrapperSharedURLLoaderFactory>(
            std::move(proxied_factory_info_));
      } else {
        factory = network_loader_factory_;
      }
    }
    url_chain_.push_back(resource_request_->url);
    *out_options = GetURLLoaderOptions(resource_request_->resource_type ==
                                       RESOURCE_TYPE_MAIN_FRAME);
    return factory;
  }
