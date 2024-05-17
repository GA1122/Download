IOThread::Globals::Globals()
    : system_request_context_leak_checker(this),
      ignore_certificate_errors(false),
      http_pipelining_enabled(false),
      testing_fixed_http_port(0),
      testing_fixed_https_port(0),
      enable_user_alternate_protocol_ports(false) {
}
