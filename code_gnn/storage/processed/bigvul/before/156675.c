void NavigateNamedFrame(const ToRenderFrameHost& caller_frame,
                        const GURL& url,
                        const std::string& name) {
  EXPECT_EQ(true, EvalJs(caller_frame,
                         JsReplace("!!window.open($1, $2)", url, name)));
}
