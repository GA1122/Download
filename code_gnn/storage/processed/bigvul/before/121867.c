void IOThread::InitializeNetworkSessionParams(
    net::HttpNetworkSession::Params* params) {
  params->host_resolver = globals_->host_resolver.get();
  params->cert_verifier = globals_->cert_verifier.get();
  params->server_bound_cert_service =
      globals_->system_server_bound_cert_service.get();
  params->transport_security_state = globals_->transport_security_state.get();
  params->ssl_config_service = globals_->ssl_config_service.get();
  params->http_auth_handler_factory = globals_->http_auth_handler_factory.get();
  params->http_server_properties =
      globals_->http_server_properties->GetWeakPtr();
  params->network_delegate = globals_->system_network_delegate.get();
  params->host_mapping_rules = globals_->host_mapping_rules.get();
  params->ignore_certificate_errors = globals_->ignore_certificate_errors;
  params->http_pipelining_enabled = globals_->http_pipelining_enabled;
  params->testing_fixed_http_port = globals_->testing_fixed_http_port;
  params->testing_fixed_https_port = globals_->testing_fixed_https_port;

  globals_->initial_max_spdy_concurrent_streams.CopyToIfSet(
      &params->spdy_initial_max_concurrent_streams);
  globals_->max_spdy_concurrent_streams_limit.CopyToIfSet(
      &params->spdy_max_concurrent_streams_limit);
  globals_->force_spdy_single_domain.CopyToIfSet(
      &params->force_spdy_single_domain);
  globals_->enable_spdy_ip_pooling.CopyToIfSet(
      &params->enable_spdy_ip_pooling);
  globals_->enable_spdy_compression.CopyToIfSet(
      &params->enable_spdy_compression);
  globals_->enable_spdy_ping_based_connection_checking.CopyToIfSet(
      &params->enable_spdy_ping_based_connection_checking);
  globals_->spdy_default_protocol.CopyToIfSet(
      &params->spdy_default_protocol);
  globals_->trusted_spdy_proxy.CopyToIfSet(
      &params->trusted_spdy_proxy);
  globals_->enable_quic.CopyToIfSet(&params->enable_quic);
  globals_->enable_quic_https.CopyToIfSet(&params->enable_quic_https);
  globals_->quic_max_packet_length.CopyToIfSet(&params->quic_max_packet_length);
  globals_->origin_to_force_quic_on.CopyToIfSet(
      &params->origin_to_force_quic_on);
  params->enable_user_alternate_protocol_ports =
      globals_->enable_user_alternate_protocol_ports;
}
