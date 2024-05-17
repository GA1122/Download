void RenderViewHostImpl::OnDidFailProvisionalLoadWithError(
    const ViewHostMsg_DidFailProvisionalLoadWithError_Params& params) {
  delegate_->DidFailProvisionalLoadWithError(this, params);
}
