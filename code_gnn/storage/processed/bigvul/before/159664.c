  void ExecuteScript(const ToRenderFrameHost& adapter, const char* script) {
    adapter.render_frame_host()->ExecuteJavaScriptForTests(
        base::UTF8ToUTF16(script));
  }
