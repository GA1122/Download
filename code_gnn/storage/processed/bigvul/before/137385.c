void RenderViewTest::ExecuteJavaScriptForTests(const char* js) {
  GetMainFrame()->ExecuteScript(WebScriptSource(WebString::FromUTF8(js)));
}
