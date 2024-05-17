void RenderProcessHostImpl::FilterURL(RenderProcessHost* rph,
                                      bool empty_allowed,
                                      GURL* url) {
  ChildProcessSecurityPolicyImpl* policy =
      ChildProcessSecurityPolicyImpl::GetInstance();

  if (empty_allowed && url->is_empty())
    return;

  if (!url->is_valid()) {
    *url = GURL(url::kAboutBlankURL);
    return;
  }

  if (!policy->CanRequestURL(rph->GetID(), *url)) {
    VLOG(1) << "Blocked URL " << url->spec();
    *url = GURL(url::kAboutBlankURL);
  }
}
