void RenderWidgetHostImpl::CopyToFindPboard() {
#if defined(OS_MACOSX)
  Send(new ViewMsg_CopyToFindPboard(GetRoutingID()));
  RecordAction(UserMetricsAction("CopyToFindPboard"));
#endif
}
