void RenderViewTest::ExecuteJavaScriptForTests(const char* js) {
  GetMainFrame()->executeScript(WebScriptSource(WebString::fromUTF8(js)));
}
