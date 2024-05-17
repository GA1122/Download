void WebLocalFrameImpl::LoadJavaScriptURL(const WebURL& url) {
  DCHECK(GetFrame());

  Document* owner_document = GetFrame()->GetDocument();

  if (!owner_document || !GetFrame()->GetPage())
    return;

  if (SchemeRegistry::ShouldTreatURLSchemeAsNotAllowingJavascriptURLs(
          owner_document->Url().Protocol()))
    return;

  String script = DecodeURLEscapeSequences(
      static_cast<const KURL&>(url).GetString().Substring(
          strlen("javascript:")));
  std::unique_ptr<UserGestureIndicator> gesture_indicator =
      Frame::NotifyUserActivation(GetFrame(), UserGestureToken::kNewGesture);
  v8::HandleScope handle_scope(ToIsolate(GetFrame()));
  v8::Local<v8::Value> result =
      GetFrame()->GetScriptController().ExecuteScriptInMainWorldAndReturnValue(
          ScriptSourceCode(script, ScriptSourceLocationType::kJavascriptUrl));
  if (result.IsEmpty() || !result->IsString())
    return;
  String script_result = ToCoreString(v8::Local<v8::String>::Cast(result));
  if (!GetFrame()->GetNavigationScheduler().LocationChangePending()) {
    GetFrame()->Loader().ReplaceDocumentWhileExecutingJavaScriptURL(
        script_result, owner_document);
  }
}
