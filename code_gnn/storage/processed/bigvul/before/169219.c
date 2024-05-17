void ExecuteScriptAsync(const ToRenderFrameHost& adapter,
                        const std::string& script) {
  adapter.render_frame_host()->ExecuteJavaScriptWithUserGestureForTests(
      base::UTF8ToUTF16(script));
}
