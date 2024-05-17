void ExtensionHelper::OnExecuteCode(
    const ExtensionMsg_ExecuteCode_Params& params) {
  WebView* webview = render_view()->GetWebView();
  WebFrame* main_frame = webview->mainFrame();
  if (!main_frame) {
    Send(new ExtensionHostMsg_ExecuteCodeFinished(
        routing_id(), params.request_id, false, ""));
    return;
  }

  SchedulerMap::iterator i = g_schedulers.Get().find(main_frame);
  if (i != g_schedulers.Get().end())
    i->second->ExecuteCode(params);
}
