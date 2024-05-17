bool RenderViewTest::ExecuteJavaScriptAndReturnIntValue(
    const string16& script,
    int* int_result) {
  v8::Handle<v8::Value> result =
      GetMainFrame()->executeScriptAndReturnValue(WebScriptSource(script));
  if (result.IsEmpty() || !result->IsInt32())
    return false;

  if (int_result)
    *int_result = result->Int32Value();

  return true;
}
