void RenderViewHostImpl::AllowBindings(int bindings_flags) {
  if (bindings_flags & BINDINGS_POLICY_WEB_UI &&
      GetProcess()->HasConnection() &&
      !ChildProcessSecurityPolicyImpl::GetInstance()->HasWebUIBindings(
          GetProcess()->GetID())) {
    RenderProcessHostImpl* process =
        static_cast<RenderProcessHostImpl*>(GetProcess());
    if (process->GetActiveViewCount() > 1)
      return;
  }

  if (GetProcess()->IsGuest()) {
    NOTREACHED() << "Never grant bindings to a guest process.";
    return;
  }

  if (bindings_flags & BINDINGS_POLICY_WEB_UI) {
    ChildProcessSecurityPolicyImpl::GetInstance()->GrantWebUIBindings(
        GetProcess()->GetID());
  }

  enabled_bindings_ |= bindings_flags;
  if (renderer_initialized_)
    Send(new ViewMsg_AllowBindings(GetRoutingID(), enabled_bindings_));
}
