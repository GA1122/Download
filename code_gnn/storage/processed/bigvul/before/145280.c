void I18NCustomBindings::GetL10nMessage(
    const v8::FunctionCallbackInfo<v8::Value>& args) {
  if (args.Length() != 3 || !args[0]->IsString()) {
    NOTREACHED() << "Bad arguments";
    return;
  }

  std::string extension_id;
  if (args[2]->IsNull() || !args[2]->IsString()) {
    return;
  } else {
    extension_id = *v8::String::Utf8Value(args[2]);
    if (extension_id.empty())
      return;
  }

  L10nMessagesMap* l10n_messages = GetL10nMessagesMap(extension_id);
  if (!l10n_messages) {
    content::RenderFrame* render_frame = context()->GetRenderFrame();
    if (!render_frame)
      return;

    L10nMessagesMap messages;
    render_frame->Send(
        new ExtensionHostMsg_GetMessageBundle(extension_id, &messages));

    ExtensionToL10nMessagesMap& l10n_messages_map =
        *GetExtensionToL10nMessagesMap();
    l10n_messages_map[extension_id] = messages;

    l10n_messages = GetL10nMessagesMap(extension_id);
  }

  std::string message_name = *v8::String::Utf8Value(args[0]);
  std::string message =
      MessageBundle::GetL10nMessage(message_name, *l10n_messages);

  v8::Isolate* isolate = args.GetIsolate();
  std::vector<std::string> substitutions;
  if (args[1]->IsArray()) {
    v8::Local<v8::Array> placeholders = v8::Local<v8::Array>::Cast(args[1]);
    uint32_t count = placeholders->Length();
    if (count > 9)
      return;
    for (uint32_t i = 0; i < count; ++i) {
      substitutions.push_back(*v8::String::Utf8Value(placeholders->Get(
          v8::Integer::New(isolate, i))));
    }
  } else if (args[1]->IsString()) {
    substitutions.push_back(*v8::String::Utf8Value(args[1]));
  }

  args.GetReturnValue().Set(v8::String::NewFromUtf8(
      isolate,
      base::ReplaceStringPlaceholders(message, substitutions, NULL).c_str()));
}
