gfx::Rect RenderViewTest::GetElementBounds(const std::string& element_id) {
  static constexpr char kGetCoordinatesScript[] =
      "(function() {"
      "  function GetCoordinates(elem) {"
      "    if (!elem)"
      "      return [ 0, 0];"
      "    var coordinates = [ elem.offsetLeft, elem.offsetTop];"
      "    var parent_coordinates = GetCoordinates(elem.offsetParent);"
      "    coordinates[0] += parent_coordinates[0];"
      "    coordinates[1] += parent_coordinates[1];"
      "    return [ Math.round(coordinates[0]),"
      "             Math.round(coordinates[1])];"
      "  };"
      "  var elem = document.getElementById('$1');"
      "  if (!elem)"
      "    return null;"
      "  var bounds = GetCoordinates(elem);"
      "  bounds[2] = Math.round(elem.offsetWidth);"
      "  bounds[3] = Math.round(elem.offsetHeight);"
      "  return bounds;"
      "})();";
  std::vector<std::string> params;
  params.push_back(element_id);
  std::string script =
      base::ReplaceStringPlaceholders(kGetCoordinatesScript, params, nullptr);

  v8::Isolate* isolate = v8::Isolate::GetCurrent();
  v8::HandleScope handle_scope(isolate);
  v8::Local<v8::Value> value = GetMainFrame()->ExecuteScriptAndReturnValue(
      WebScriptSource(WebString::FromUTF8(script)));
  if (value.IsEmpty() || !value->IsArray())
    return gfx::Rect();

  v8::Local<v8::Array> array = value.As<v8::Array>();
  if (array->Length() != 4)
    return gfx::Rect();
  std::vector<int> coords;
  for (int i = 0; i < 4; ++i) {
    v8::Local<v8::Number> index = v8::Number::New(isolate, i);
    v8::Local<v8::Value> value = array->Get(index);
    if (value.IsEmpty() || !value->IsInt32())
      return gfx::Rect();
    coords.push_back(value->Int32Value());
  }
  return gfx::Rect(coords[0], coords[1], coords[2], coords[3]);
}
