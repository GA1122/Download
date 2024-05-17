void NavigationRequest::OnWillProcessResponseChecksComplete(
    NavigationThrottle::ThrottleCheckResult result) {
  DCHECK(result.action() != NavigationThrottle::DEFER);

  if (result.action() == NavigationThrottle::PROCEED) {
    if (is_download_ &&
        base::FeatureList::IsEnabled(network::features::kNetworkService)) {
      auto resource_request = std::make_unique<network::ResourceRequest>();
      resource_request->url = common_params_.url;
      resource_request->method = common_params_.method;
      resource_request->request_initiator = begin_params_->initiator_origin;
      resource_request->referrer = common_params_.referrer.url;

      BrowserContext* browser_context =
          frame_tree_node_->navigator()->GetController()->GetBrowserContext();
      DownloadManagerImpl* download_manager = static_cast<DownloadManagerImpl*>(
          BrowserContext::GetDownloadManager(browser_context));
      download_manager->InterceptNavigation(
          std::move(resource_request), navigation_handle_->GetRedirectChain(),
          common_params_.suggested_filename, response_,
          std::move(url_loader_client_endpoints_), ssl_info_.cert_status,
          frame_tree_node_->frame_tree_node_id());

      OnRequestFailed(false, net::ERR_ABORTED, base::nullopt);
      return;
    }

    if (!base::FeatureList::IsEnabled(network::features::kNetworkService)) {
      if (url_loader_client_endpoints_) {
        network::mojom::URLLoaderPtr url_loader(
            std::move(url_loader_client_endpoints_->url_loader));
        url_loader->ProceedWithResponse();
        url_loader_client_endpoints_->url_loader = url_loader.PassInterface();
      } else {
        loader_->ProceedWithResponse();
      }
    }
  }

  if (result.action() == NavigationThrottle::CANCEL_AND_IGNORE ||
      result.action() == NavigationThrottle::CANCEL ||
      !response_should_be_rendered_) {
    if (!response_should_be_rendered_) {
      OnRequestFailedInternal(false, net::ERR_ABORTED, base::nullopt, true,
                              base::nullopt);
      return;
    }

    DCHECK(result.action() == NavigationThrottle::CANCEL ||
           result.net_error_code() == net::ERR_ABORTED);
    OnRequestFailedInternal(false, result.net_error_code(), base::nullopt, true,
                            result.error_page_content());

    return;
  }

  if (result.action() == NavigationThrottle::BLOCK_RESPONSE) {
    DCHECK_EQ(net::ERR_BLOCKED_BY_RESPONSE, result.net_error_code());
    OnRequestFailedInternal(false, result.net_error_code(), base::nullopt, true,
                            result.error_page_content());
    return;
  }

  CommitNavigation();

}
