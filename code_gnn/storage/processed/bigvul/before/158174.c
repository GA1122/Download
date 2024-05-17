std::unique_ptr<NavigationRequestInfo> CreateNavigationRequestInfoForRedirect(
    const NavigationRequestInfo& previous_request_info,
    const network::ResourceRequest& updated_resource_request) {
  DCHECK(!base::FeatureList::IsEnabled(network::features::kNetworkService));
  DCHECK(IsLoaderInterceptionEnabled());

  CommonNavigationParams new_common_params =
      previous_request_info.common_params;
  new_common_params.url = updated_resource_request.url;
  new_common_params.referrer =
      Referrer(updated_resource_request.referrer,
               Referrer::NetReferrerPolicyToBlinkReferrerPolicy(
                   updated_resource_request.referrer_policy));
  new_common_params.method = updated_resource_request.method;
  new_common_params.post_data = updated_resource_request.request_body;

  mojom::BeginNavigationParamsPtr new_begin_params =
      previous_request_info.begin_params.Clone();
  new_begin_params->headers = updated_resource_request.headers.ToString();

  return std::make_unique<NavigationRequestInfo>(
      std::move(new_common_params), std::move(new_begin_params),
      updated_resource_request.site_for_cookies,
      previous_request_info.is_main_frame,
      previous_request_info.parent_is_main_frame,
      previous_request_info.are_ancestors_secure,
      previous_request_info.frame_tree_node_id,
      previous_request_info.is_for_guests_only,
      previous_request_info.report_raw_headers,
      previous_request_info.is_prerendering,
      previous_request_info.upgrade_if_insecure,
      nullptr  ,
      previous_request_info.devtools_navigation_token,
      previous_request_info.devtools_frame_token);
}
