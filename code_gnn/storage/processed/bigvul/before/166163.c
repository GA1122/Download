bool RenderFrameHostImpl::ValidateDidCommitParams(
    FrameHostMsg_DidCommitProvisionalLoad_Params* validated_params) {
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
    if (GetNavigationHandle() && GetNavigationHandle()->GetNetErrorCode() ==
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

  if (!is_permitted_error_page && !CanCommitURL(validated_params->url)) {
    VLOG(1) << "Blocked URL " << validated_params->url.spec();
    LogRendererKillCrashKeys(GetSiteInstance()->GetSiteURL());

    bad_message::ReceivedBadMessage(process,
                                    bad_message::RFH_CAN_COMMIT_URL_BLOCKED);
    return false;
  }

  if (!CanCommitOrigin(validated_params->origin, validated_params->url)) {
    DEBUG_ALIAS_FOR_ORIGIN(origin_debug_alias, validated_params->origin);
    LogRendererKillCrashKeys(GetSiteInstance()->GetSiteURL());

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
