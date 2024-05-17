  URLLoaderRequestController(
      std::vector<std::unique_ptr<NavigationLoaderInterceptor>>
          initial_interceptors,
      std::unique_ptr<network::ResourceRequest> resource_request,
      ResourceContext* resource_context,
      const GURL& url,
      bool is_main_frame,
      network::mojom::URLLoaderFactoryRequest proxied_factory_request,
      network::mojom::URLLoaderFactoryPtrInfo proxied_factory_info,
      std::set<std::string> known_schemes,
      bool bypass_redirect_checks,
      const base::WeakPtr<NavigationURLLoaderImpl>& owner)
      : interceptors_(std::move(initial_interceptors)),
        resource_request_(std::move(resource_request)),
        resource_context_(resource_context),
        url_(url),
        is_main_frame_(is_main_frame),
        owner_(owner),
        response_loader_binding_(this),
        proxied_factory_request_(std::move(proxied_factory_request)),
        proxied_factory_info_(std::move(proxied_factory_info)),
        known_schemes_(std::move(known_schemes)),
        bypass_redirect_checks_(bypass_redirect_checks),
        weak_factory_(this) {}
