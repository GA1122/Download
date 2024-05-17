  void Initialize(network::mojom::NetworkContextRequest network_context_request,
                  scoped_refptr<net::URLRequestContextGetter> context_getter) {
    DCHECK_CURRENTLY_ON(BrowserThread::IO);
    context_getter_ = std::move(context_getter);
    network_context_ = std::make_unique<network::NetworkContext>(
        GetNetworkServiceImpl(), std::move(network_context_request),
        context_getter_->GetURLRequestContext());
  }
