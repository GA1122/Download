void ResourceDispatcherHostImpl::OnSwapOutACK(
  const ViewMsg_SwapOut_Params& params) {
  HandleSwapOutACK(params, false);
}
