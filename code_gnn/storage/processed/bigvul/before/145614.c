NavigationRequest::CheckCredentialedSubresource() const {
  if (frame_tree_node_->IsMainFrame())
    return CredentialedSubresourceCheckResult::ALLOW_REQUEST;

  if (!common_params_.url.has_username() && !common_params_.url.has_password())
    return CredentialedSubresourceCheckResult::ALLOW_REQUEST;

  FrameTreeNode* parent_ftn = frame_tree_node_->parent();
  DCHECK(parent_ftn);
  const GURL& parent_url = parent_ftn->current_url();
  if (url::Origin::Create(parent_url)
          .IsSameOriginWith(url::Origin::Create(common_params_.url)) &&
      parent_url.username() == common_params_.url.username() &&
      parent_url.password() == common_params_.url.password()) {
    return CredentialedSubresourceCheckResult::ALLOW_REQUEST;
  }

  RenderFrameHostImpl* parent = parent_ftn->current_frame_host();
  DCHECK(parent);
  const char* console_message =
      "Subresource requests whose URLs contain embedded credentials (e.g. "
      "`https://user:pass@host/`) are blocked. See "
      "https://www.chromestatus.com/feature/5669008342777856 for more "
      "details.";
  parent->AddMessageToConsole(CONSOLE_MESSAGE_LEVEL_WARNING, console_message);

  if (!base::FeatureList::IsEnabled(features::kBlockCredentialedSubresources))
    return CredentialedSubresourceCheckResult::ALLOW_REQUEST;

  return CredentialedSubresourceCheckResult::BLOCK_REQUEST;
}
