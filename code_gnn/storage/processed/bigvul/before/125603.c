void RenderViewHostImpl::ExecuteJavascriptInWebFrameCallbackResult(
     const string16& frame_xpath,
     const string16& jscript,
     const JavascriptResultCallback& callback) {
  static int next_id = 1;
  int key = next_id++;
  Send(new ViewMsg_ScriptEvalRequest(GetRoutingID(), frame_xpath, jscript,
                                     key, true));
  javascript_callbacks_.insert(std::make_pair(key, callback));
}
