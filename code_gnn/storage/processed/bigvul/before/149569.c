  void ResetNetworkState() {
    auto* network_context = content::BrowserContext::GetDefaultStoragePartition(
                                browser()->profile())
                                ->GetNetworkContext();
    base::RunLoop clear_host_cache_loop;
    base::RunLoop close_all_connections_loop;
    network_context->ClearHostCache(nullptr,
                                    clear_host_cache_loop.QuitClosure());
    network_context->CloseAllConnections(
        close_all_connections_loop.QuitClosure());
    clear_host_cache_loop.Run();
    close_all_connections_loop.Run();

    connection_tracker()->ResetCounts();
    preconnecting_server_connection_tracker_->ResetCounts();
  }
