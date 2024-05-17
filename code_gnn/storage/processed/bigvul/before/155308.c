ChromeContentBrowserClient::CreateURLLoaderFactoryForNetworkRequests(
    content::RenderProcessHost* process,
    network::mojom::NetworkContext* network_context,
    network::mojom::TrustedURLLoaderHeaderClientPtrInfo* header_client,
    const url::Origin& request_initiator) {
#if BUILDFLAG(ENABLE_EXTENSIONS)
  return ChromeContentBrowserClientExtensionsPart::
      CreateURLLoaderFactoryForNetworkRequests(
          process, network_context, header_client, request_initiator);
#else
  return network::mojom::URLLoaderFactoryPtrInfo();
#endif
}
