void CheckNavigationEntryMatchLoadParams(
    const NavigationController::LoadURLParams& load_params,
    NavigationEntryImpl* entry) {
  EXPECT_EQ(load_params.url, entry->GetURL());
  EXPECT_EQ(load_params.referrer.url, entry->GetReferrer().url);
  EXPECT_EQ(load_params.referrer.policy, entry->GetReferrer().policy);
  EXPECT_TRUE(ui::PageTransitionTypeIncludingQualifiersIs(
      entry->GetTransitionType(), load_params.transition_type));
  std::string extra_headers_crlf;
  base::ReplaceChars(load_params.extra_headers, "\n", "\r\n",
                     &extra_headers_crlf);
  EXPECT_EQ(extra_headers_crlf, entry->extra_headers());

  EXPECT_EQ(load_params.is_renderer_initiated, entry->is_renderer_initiated());
  EXPECT_EQ(load_params.base_url_for_data_url, entry->GetBaseURLForDataURL());
  if (!load_params.virtual_url_for_data_url.is_empty()) {
    EXPECT_EQ(load_params.virtual_url_for_data_url, entry->GetVirtualURL());
  }
#if defined(OS_ANDROID)
  EXPECT_EQ(load_params.data_url_as_string, entry->GetDataURLAsString());
#endif
  if (NavigationController::UA_OVERRIDE_INHERIT !=
      load_params.override_user_agent) {
    bool should_override = (NavigationController::UA_OVERRIDE_TRUE ==
        load_params.override_user_agent);
    EXPECT_EQ(should_override, entry->GetIsOverridingUserAgent());
  }
  EXPECT_EQ(load_params.post_data, entry->GetPostData());
}
