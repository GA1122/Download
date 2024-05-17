XSSAuditor::XSSAuditor()
    : is_enabled_(false),
      xss_protection_(kFilterReflectedXSS),
      did_send_valid_xss_protection_header_(false),
      state_(kUninitialized),
      script_tag_found_in_request_(false),
      script_tag_nesting_level_(0),
      encoding_(UTF8Encoding()) {
}
