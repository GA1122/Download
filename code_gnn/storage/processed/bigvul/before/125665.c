void RenderViewHostImpl::OnNavigate(const IPC::Message& msg) {
  PickleIterator iter(msg);
  ViewHostMsg_FrameNavigate_Params validated_params;
  if (!IPC::ParamTraits<ViewHostMsg_FrameNavigate_Params>::
      Read(&msg, &iter, &validated_params))
    return;

  if (is_waiting_for_beforeunload_ack_ &&
      unload_ack_is_for_cross_site_transition_ &&
      PageTransitionIsMainFrame(validated_params.transition)) {
    OnShouldCloseACK(true, send_should_close_start_time_,
                        base::TimeTicks::Now());
    return;
  }

  if (is_waiting_for_unload_ack_)
    return;

  RenderProcessHost* process = GetProcess();

  const CommandLine& command_line = *CommandLine::ForCurrentProcess();
  if (command_line.HasSwitch(switches::kSitePerProcess) &&
      static_cast<SiteInstanceImpl*>(GetSiteInstance())->HasSite() &&
      validated_params.url != GURL(chrome::kAboutBlankURL)) {
    if (!SiteInstance::IsSameWebSite(GetSiteInstance()->GetBrowserContext(),
                                     GetSiteInstance()->GetSiteURL(),
                                     validated_params.url) ||
        static_cast<SiteInstanceImpl*>(GetSiteInstance())->
            HasWrongProcessForURL(validated_params.url)) {
    }
  }

  ChildProcessSecurityPolicyImpl* policy =
      ChildProcessSecurityPolicyImpl::GetInstance();
  FilterURL(policy, process, false, &validated_params.url);
  FilterURL(policy, process, true, &validated_params.referrer.url);
  for (std::vector<GURL>::iterator it(validated_params.redirects.begin());
      it != validated_params.redirects.end(); ++it) {
    FilterURL(policy, process, false, &(*it));
  }
  FilterURL(policy, process, true, &validated_params.searchable_form_url);
  FilterURL(policy, process, true, &validated_params.password_form.origin);
  FilterURL(policy, process, true, &validated_params.password_form.action);

  delegate_->DidNavigate(this, validated_params);

}
