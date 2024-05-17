  void CheckPluginAndContinueOnReceiveResponse(
      const network::ResourceResponseHead& head,
      network::mojom::URLLoaderClientEndpointsPtr url_loader_client_endpoints,
      bool is_download_if_not_handled_by_plugin,
      const std::vector<WebPluginInfo>& plugins) {
    bool stale;
    WebPluginInfo plugin;
    bool has_plugin = PluginService::GetInstance()->GetPluginInfo(
        -1  , -1  , resource_context_,
        resource_request_->url, url::Origin(), head.mime_type,
        false  , &stale, &plugin, nullptr);

    if (stale) {
      PluginService::GetInstance()->GetPlugins(base::BindOnce(
          &URLLoaderRequestController::CheckPluginAndContinueOnReceiveResponse,
          weak_factory_.GetWeakPtr(), head,
          std::move(url_loader_client_endpoints),
          is_download_if_not_handled_by_plugin));
      return;
    }

    bool is_download = !has_plugin && is_download_if_not_handled_by_plugin;

    CallOnReceivedResponse(head, std::move(url_loader_client_endpoints),
                           nullptr, is_download, false  ,
                           PREVIEWS_OFF  );
  }
