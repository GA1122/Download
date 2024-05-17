void RenderViewHostImpl::OnUpdateTargetURL(int32 page_id, const GURL& url) {
  if (!is_swapped_out_)
    delegate_->UpdateTargetURL(page_id, url);

  Send(new ViewMsg_UpdateTargetURL_ACK(GetRoutingID()));
}
