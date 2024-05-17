void RenderProcessHostImpl::SimulateSwapOutACK(
    const ViewMsg_SwapOut_Params& params) {
  widget_helper_->SimulateSwapOutACK(params);
}
