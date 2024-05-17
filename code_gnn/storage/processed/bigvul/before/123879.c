static void NotifyTimezoneChange(WebKit::WebFrame* frame) {
  v8::HandleScope handle_scope;
  v8::Context::Scope context_scope(frame->mainWorldScriptContext());
  v8::Date::DateTimeConfigurationChangeNotification();
  WebKit::WebFrame* child = frame->firstChild();
  for (; child; child = child->nextSibling())
    NotifyTimezoneChange(child);
}
