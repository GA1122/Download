  FrozenState(ReferrerPolicy referrer_policy,
              const String& outgoing_referrer,
              const KURL& url,
              RefPtr<SecurityOrigin> security_origin,
              RefPtr<const SecurityOrigin> parent_security_origin,
              const Optional<WebAddressSpace>& address_space,
              const ContentSecurityPolicy* content_security_policy,
              KURL site_for_cookies,
              RefPtr<SecurityOrigin> requestor_origin,
              RefPtr<SecurityOrigin> requestor_origin_for_frame_loading,
              const ClientHintsPreferences& client_hints_preferences,
              float device_pixel_ratio,
              const String& user_agent,
              bool is_main_frame,
              bool is_svg_image_chrome_client)
      : referrer_policy(referrer_policy),
        outgoing_referrer(outgoing_referrer),
        url(url),
        security_origin(std::move(security_origin)),
        parent_security_origin(std::move(parent_security_origin)),
        address_space(address_space),
        content_security_policy(content_security_policy),
        site_for_cookies(site_for_cookies),
        requestor_origin(requestor_origin),
        requestor_origin_for_frame_loading(requestor_origin_for_frame_loading),
        client_hints_preferences(client_hints_preferences),
        device_pixel_ratio(device_pixel_ratio),
        user_agent(user_agent),
        is_main_frame(is_main_frame),
        is_svg_image_chrome_client(is_svg_image_chrome_client) {}