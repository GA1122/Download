void RenderViewHostImpl::SyncRendererPrefs() {
  Send(new ViewMsg_SetRendererPrefs(GetRoutingID(),
                                    delegate_->GetRendererPrefs(
                                        GetProcess()->GetBrowserContext())));
}
