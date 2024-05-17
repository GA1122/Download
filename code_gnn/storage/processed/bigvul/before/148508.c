void WebContentsImpl::OnUnregisterProtocolHandler(RenderFrameHostImpl* source,
                                                  const std::string& protocol,
                                                  const GURL& url,
                                                  bool user_gesture) {
  if (!delegate_)
    return;

  ChildProcessSecurityPolicyImpl* policy =
      ChildProcessSecurityPolicyImpl::GetInstance();
  if (policy->IsPseudoScheme(protocol))
    return;

  delegate_->UnregisterProtocolHandler(this, protocol, url, user_gesture);
}
