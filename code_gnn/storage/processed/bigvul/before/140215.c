WebResourceService::WebResourceService(
    PrefService* prefs,
    const GURL& web_resource_server,
    const std::string& application_locale,
    const char* last_update_time_pref_name,
    int start_fetch_delay_ms,
    int cache_update_delay_ms,
    net::URLRequestContextGetter* request_context,
    const char* disable_network_switch,
    const ParseJSONCallback& parse_json_callback)
    : prefs_(prefs),
      resource_request_allowed_notifier_(prefs, disable_network_switch),
      in_fetch_(false),
      web_resource_server_(web_resource_server),
      application_locale_(application_locale),
      last_update_time_pref_name_(last_update_time_pref_name),
      start_fetch_delay_ms_(start_fetch_delay_ms),
      cache_update_delay_ms_(cache_update_delay_ms),
      request_context_(request_context),
      parse_json_callback_(parse_json_callback),
      weak_ptr_factory_(this) {
  resource_request_allowed_notifier_.Init(this);
  DCHECK(prefs);
}
