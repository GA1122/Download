void BrowserRenderProcessHost::CrossSiteClosePageACK(
    const ViewMsg_ClosePage_Params& params) {
  widget_helper_->CrossSiteClosePageACK(params);
}
