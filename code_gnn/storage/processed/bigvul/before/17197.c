  BrowserContextSharedIOData(BrowserContextIOData* context,
                             const BrowserContext::Params& params)
      : path(params.path),
        cache_path(params.cache_path),
        max_cache_size_hint(params.max_cache_size_hint),
        cookie_policy(net::StaticCookiePolicy::ALLOW_ALL_COOKIES),
        session_cookie_mode(params.session_cookie_mode),
        popup_blocker_enabled(true),
        host_mapping_rules(params.host_mapping_rules),
        user_agent_settings(new UserAgentSettingsIOData(context)),
        do_not_track(false) {}
