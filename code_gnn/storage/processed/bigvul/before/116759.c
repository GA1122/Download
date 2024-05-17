void RenderViewTest::ExecuteJavaScript(const char* js) {
  GetMainFrame()->executeScript(WebScriptSource(WebString::fromUTF8(js)));
}
