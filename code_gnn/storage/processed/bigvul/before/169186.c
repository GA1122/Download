bool RenderFrameHostImpl::ValidateDidCommitParams(
    FrameHostMsg_DidCommitProvisionalLoad_Params* validated_params) {
  RenderProcessHost* process = GetProcess();

  if (!CanCommitURL(validated_params->url)) {
    VLOG(1) << "Blocked URL " << validated_params->url.spec();
    bad_message::ReceivedBadMessage(process,
                                    bad_message::RFH_CAN_COMMIT_URL_BLOCKED);
    return false;
  }

  if (!CanCommitOrigin(validated_params->origin, validated_params->url)) {
    DEBUG_ALIAS_FOR_ORIGIN(origin_debug_alias, validated_params->origin);
    bad_message::ReceivedBadMessage(process,
                                    bad_message::RFH_INVALID_ORIGIN_ON_COMMIT);
    return false;
  }

  process->FilterURL(false, &validated_params->url);
  process->FilterURL(true, &validated_params->referrer.url);
  for (std::vector<GURL>::iterator it(validated_params->redirects.begin());
       it != validated_params->redirects.end(); ++it) {
    process->FilterURL(false, &(*it));
  }
  process->FilterURL(true, &validated_params->searchable_form_url);

  if (!CanAccessFilesOfPageState(validated_params->page_state)) {
    bad_message::ReceivedBadMessage(
        process, bad_message::RFH_CAN_ACCESS_FILES_OF_PAGE_STATE);
    return false;
  }

  return true;
}
