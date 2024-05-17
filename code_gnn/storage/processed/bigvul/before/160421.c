bool RenderFrameHostImpl::CanExecuteJavaScript() {
#if defined(OS_ANDROID)
  if (g_allow_injecting_javascript)
    return true;
#endif
  return !frame_tree_node_->current_url().is_valid() ||
         frame_tree_node_->current_url().SchemeIs(kChromeDevToolsScheme) ||
         ChildProcessSecurityPolicyImpl::GetInstance()->HasWebUIBindings(
             GetProcess()->GetID()) ||
         (frame_tree_node_->current_url().spec() == url::kAboutBlankURL) ||
         (delegate_->GetAsWebContents() == nullptr);
}
