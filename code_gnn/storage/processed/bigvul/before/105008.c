HttpBridge::RequestContext::RequestContext(
    net::URLRequestContext* baseline_context)
    : baseline_context_(baseline_context) {

  set_cookie_store(new net::CookieMonster(NULL, NULL));

  set_host_resolver(baseline_context->host_resolver());
  set_proxy_service(baseline_context->proxy_service());
  set_ssl_config_service(baseline_context->ssl_config_service());

  net::HttpNetworkSession* session =
      baseline_context->http_transaction_factory()->GetSession();
  DCHECK(session);
  set_http_transaction_factory(new net::HttpNetworkLayer(session));

  set_accept_language(baseline_context->accept_language());
  set_accept_charset(baseline_context->accept_charset());

  set_user_agent(webkit_glue::GetUserAgent(GURL()));

  set_net_log(baseline_context->net_log());
}
