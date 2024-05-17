static void set_api_wants_to_log(bool value) {
  logging_enabled_via_api = value;
  update_logging();
}
