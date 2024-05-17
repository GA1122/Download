void ChromeContentBrowserClient::WillCreateWebSocket(
    content::RenderFrameHost* frame,
    network::mojom::WebSocketRequest* request,
    network::mojom::AuthenticationHandlerPtr* auth_handler) {
#if BUILDFLAG(ENABLE_EXTENSIONS)
  auto* web_request_api =
      extensions::BrowserContextKeyedAPIFactory<extensions::WebRequestAPI>::Get(
          frame->GetProcess()->GetBrowserContext());

  if (!web_request_api)
    return;

  web_request_api->MaybeProxyWebSocket(frame, request, auth_handler);
#endif
}