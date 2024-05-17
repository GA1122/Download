void BrowserRenderProcessHost::CrossSiteSwapOutACK(
    const ViewMsg_SwapOut_Params& params) {
  widget_helper_->CrossSiteSwapOutACK(params);
}
