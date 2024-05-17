void ResourceDispatcherHostImpl::OnSimulateSwapOutACK(
    const ViewMsg_SwapOut_Params& params) {
  HandleSwapOutACK(params, true);
}
