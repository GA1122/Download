void RenderFrameHostImpl::MaybeGenerateCrashReport(
    base::TerminationStatus status) {
  if (!last_committed_url_.SchemeIsHTTPOrHTTPS())
    return;

  if (!frame_tree_node_->IsMainFrame() && !IsCrossProcessSubframe())
    return;

  std::string reason;
  switch (status) {
    case base::TERMINATION_STATUS_ABNORMAL_TERMINATION:
    case base::TERMINATION_STATUS_PROCESS_CRASHED:
      break;
    case base::TERMINATION_STATUS_OOM:
#if defined(OS_CHROMEOS)
    case base::TERMINATION_STATUS_PROCESS_WAS_KILLED_BY_OOM:
#endif
#if defined(OS_ANDROID)
    case base::TERMINATION_STATUS_OOM_PROTECTED:
#endif
      reason = "oom";
      break;
    default:
      return;
  }

  auto body = base::DictionaryValue();
  if (!reason.empty())
    body.SetString("reason", reason);

  GetProcess()->GetStoragePartition()->GetNetworkContext()->QueueReport(
      "crash"  , "default"  , last_committed_url_,
      base::nullopt, std::move(body));
}
