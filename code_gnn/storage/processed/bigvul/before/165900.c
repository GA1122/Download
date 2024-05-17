void FillNavigationParams(const CommonNavigationParams& common_params,
                          const RequestNavigationParams& request_params,
                          blink::WebNavigationParams* navigation_params) {
  navigation_params->navigation_timings = BuildNavigationTimings(
      common_params.navigation_start, request_params.navigation_timing,
      common_params.input_start);

  if (common_params.source_location.has_value()) {
    blink::WebSourceLocation source_location;
    source_location.url =
        WebString::FromLatin1(common_params.source_location->url);
    source_location.line_number = common_params.source_location->line_number;
    source_location.column_number =
        common_params.source_location->column_number;
    navigation_params->source_location = source_location;
  }

  navigation_params->is_user_activated =
      request_params.was_activated == WasActivatedOption::kYes;
}
