bool RenderFrameHostImpl::CanCommitOrigin(
    const url::Origin& origin,
    const GURL& url) {
  if (base::CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kDisableWebSecurity)) {
    return true;
  }

  if (origin.opaque())
    return true;

  if (url.IsStandard() && !origin.IsSameOriginWith(url::Origin::Create(url)))
    return false;

  GURL origin_url = origin.GetURL();

  auto* policy = ChildProcessSecurityPolicyImpl::GetInstance();
  if (!policy->CanAccessDataForOrigin(GetProcess()->GetID(), origin))
    return false;

  return CanCommitURL(origin_url);
}
