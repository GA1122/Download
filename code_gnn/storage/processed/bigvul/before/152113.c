bool RenderFrameHostImpl::ValidateDidCommitParams(
    NavigationRequest* navigation_request,
    FrameHostMsg_DidCommitProvisionalLoad_Params* validated_params,
    bool is_same_document_navigation) {
  RenderProcessHost* process = GetProcess();

  bool is_permitted_error_page = false;
  if (SiteIsolationPolicy::IsErrorPageIsolationEnabled(
          frame_tree_node_->IsMainFrame())) {
    if (site_instance_->GetSiteURL() == GURL(content::kUnreachableWebDataURL)) {
      if (!validated_params->url_is_unreachable) {
        DEBUG_ALIAS_FOR_ORIGIN(origin_debug_alias, validated_params->origin);
        bad_message::ReceivedBadMessage(
            process, bad_message::RFH_ERROR_PROCESS_NON_ERROR_COMMIT);
        return false;
      }

      if (!validated_params->origin.opaque()) {
        DEBUG_ALIAS_FOR_ORIGIN(origin_debug_alias, validated_params->origin);
        bad_message::ReceivedBadMessage(
            process, bad_message::RFH_ERROR_PROCESS_NON_UNIQUE_ORIGIN_COMMIT);
        return false;
      }

      is_permitted_error_page = true;
    }
  } else {
    if (navigation_request &&
        navigation_request->navigation_handle()->GetNetErrorCode() ==
            net::ERR_BLOCKED_BY_CLIENT) {
      if (!validated_params->origin.opaque()) {
        DEBUG_ALIAS_FOR_ORIGIN(origin_debug_alias, validated_params->origin);
        bad_message::ReceivedBadMessage(
            process, bad_message::RFH_ERROR_PROCESS_NON_UNIQUE_ORIGIN_COMMIT);
        return false;
      }

      is_permitted_error_page = true;
    }
  }

  bool bypass_checks_for_file_scheme = false;
  if (validated_params->origin.scheme() == url::kFileScheme) {
    WebPreferences prefs = render_view_host_->GetWebkitPreferences();
    if (prefs.allow_universal_access_from_file_urls)
      bypass_checks_for_file_scheme = true;
  }

  if (!is_permitted_error_page && !bypass_checks_for_file_scheme &&
      !CanCommitURL(validated_params->url)) {
    VLOG(1) << "Blocked URL " << validated_params->url.spec();
    LogRendererKillCrashKeys(GetSiteInstance()->GetSiteURL());

    auto bool_to_crash_key = [](bool b) { return b ? "true" : "false"; };
    base::debug::SetCrashKeyString(
        base::debug::AllocateCrashKeyString("is_same_document",
                                            base::debug::CrashKeySize::Size32),
        bool_to_crash_key(is_same_document_navigation));

    base::debug::SetCrashKeyString(
        base::debug::AllocateCrashKeyString("is_main_frame",
                                            base::debug::CrashKeySize::Size32),
        bool_to_crash_key(frame_tree_node_->IsMainFrame()));

    base::debug::SetCrashKeyString(
        base::debug::AllocateCrashKeyString("is_cross_process_subframe",
                                            base::debug::CrashKeySize::Size32),
        bool_to_crash_key(IsCrossProcessSubframe()));

    base::debug::SetCrashKeyString(
        base::debug::AllocateCrashKeyString("site_lock",
                                            base::debug::CrashKeySize::Size256),
        GetSiteInstance()->lock_url().spec());

    if (navigation_request && navigation_request->navigation_handle()) {
      NavigationHandleImpl* handle = navigation_request->navigation_handle();
      base::debug::SetCrashKeyString(
          base::debug::AllocateCrashKeyString(
              "is_renderer_initiated", base::debug::CrashKeySize::Size32),
          bool_to_crash_key(handle->IsRendererInitiated()));

      base::debug::SetCrashKeyString(
          base::debug::AllocateCrashKeyString(
              "is_server_redirect", base::debug::CrashKeySize::Size32),
          bool_to_crash_key(handle->WasServerRedirect()));

      base::debug::SetCrashKeyString(
          base::debug::AllocateCrashKeyString(
              "is_form_submission", base::debug::CrashKeySize::Size32),
          bool_to_crash_key(handle->IsFormSubmission()));

      base::debug::SetCrashKeyString(
          base::debug::AllocateCrashKeyString(
              "is_error_page", base::debug::CrashKeySize::Size32),
          bool_to_crash_key(handle->IsErrorPage()));

      base::debug::SetCrashKeyString(
          base::debug::AllocateCrashKeyString(
              "net_error_code", base::debug::CrashKeySize::Size32),
          base::NumberToString(navigation_request->net_error()));

      base::debug::SetCrashKeyString(
          base::debug::AllocateCrashKeyString(
              "initiator_origin", base::debug::CrashKeySize::Size64),
          handle->GetInitiatorOrigin()
              ? handle->GetInitiatorOrigin()->GetDebugString()
              : "none");

      base::debug::SetCrashKeyString(
          base::debug::AllocateCrashKeyString(
              "starting_site_instance", base::debug::CrashKeySize::Size64),
          handle->GetStartingSiteInstance()->GetSiteURL().spec());
    }

    bad_message::ReceivedBadMessage(process,
                                    bad_message::RFH_CAN_COMMIT_URL_BLOCKED);
    return false;
  }

  if (!bypass_checks_for_file_scheme &&
      !CanCommitOrigin(validated_params->origin, validated_params->url)) {
    DEBUG_ALIAS_FOR_ORIGIN(origin_debug_alias, validated_params->origin);
    LogRendererKillCrashKeys(GetSiteInstance()->GetSiteURL());

    auto bool_to_crash_key = [](bool b) { return b ? "true" : "false"; };
    base::debug::SetCrashKeyString(
        base::debug::AllocateCrashKeyString("is_same_document",
                                            base::debug::CrashKeySize::Size32),
        bool_to_crash_key(is_same_document_navigation));

    base::debug::SetCrashKeyString(
        base::debug::AllocateCrashKeyString("is_subframe",
                                            base::debug::CrashKeySize::Size32),
        bool_to_crash_key(!frame_tree_node_->IsMainFrame()));

    base::debug::SetCrashKeyString(
        base::debug::AllocateCrashKeyString("is_active",
                                            base::debug::CrashKeySize::Size32),
        bool_to_crash_key(is_active()));

    base::debug::SetCrashKeyString(
        base::debug::AllocateCrashKeyString("is_current",
                                            base::debug::CrashKeySize::Size32),
        bool_to_crash_key(IsCurrent()));

    base::debug::SetCrashKeyString(
        base::debug::AllocateCrashKeyString("is_cross_process_subframe",
                                            base::debug::CrashKeySize::Size32),
        bool_to_crash_key(IsCrossProcessSubframe()));

    if (navigation_request && navigation_request->navigation_handle()) {
      NavigationHandleImpl* handle = navigation_request->navigation_handle();
      base::debug::SetCrashKeyString(
          base::debug::AllocateCrashKeyString(
              "is_renderer_initiated", base::debug::CrashKeySize::Size32),
          bool_to_crash_key(handle->IsRendererInitiated()));

      base::debug::SetCrashKeyString(
          base::debug::AllocateCrashKeyString(
              "is_server_redirect", base::debug::CrashKeySize::Size32),
          bool_to_crash_key(handle->WasServerRedirect()));

      base::debug::SetCrashKeyString(
          base::debug::AllocateCrashKeyString(
              "is_form_submission", base::debug::CrashKeySize::Size32),
          bool_to_crash_key(handle->IsFormSubmission()));

      base::debug::SetCrashKeyString(
          base::debug::AllocateCrashKeyString(
              "is_error_page", base::debug::CrashKeySize::Size32),
          bool_to_crash_key(handle->IsErrorPage()));

      base::debug::SetCrashKeyString(
          base::debug::AllocateCrashKeyString(
              "initiator_origin", base::debug::CrashKeySize::Size64),
          handle->GetInitiatorOrigin()
              ? handle->GetInitiatorOrigin()->GetDebugString()
              : "none");

      base::debug::SetCrashKeyString(
          base::debug::AllocateCrashKeyString(
              "starting_site_instance", base::debug::CrashKeySize::Size64),
          handle->GetStartingSiteInstance()->GetSiteURL().spec());
    }

    bad_message::ReceivedBadMessage(process,
                                    bad_message::RFH_INVALID_ORIGIN_ON_COMMIT);
    return false;
  }

  process->FilterURL(false, &validated_params->url);
  process->FilterURL(true, &validated_params->referrer.url);
  for (auto it(validated_params->redirects.begin());
       it != validated_params->redirects.end(); ++it) {
    process->FilterURL(false, &(*it));
  }

  if (!CanAccessFilesOfPageState(validated_params->page_state)) {
    bad_message::ReceivedBadMessage(
        process, bad_message::RFH_CAN_ACCESS_FILES_OF_PAGE_STATE);
    return false;
  }

  return true;
}
