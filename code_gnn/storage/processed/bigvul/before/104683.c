   static v8::Handle<v8::Value> GetCurrentPageActions(
       const v8::Arguments& args) {
     ExtensionImpl* v8_extension = GetFromArguments<ExtensionImpl>(args);
    std::string extension_id = *v8::String::Utf8Value(args[0]->ToString());
    const ::Extension* extension =
        v8_extension->extension_dispatcher_->extensions()->GetByID(
            extension_id);
    CHECK(extension);

    v8::Local<v8::Array> page_action_vector = v8::Array::New();
    if (extension->page_action()) {
      std::string id = extension->page_action()->id();
      page_action_vector->Set(v8::Integer::New(0),
                              v8::String::New(id.c_str(), id.size()));
    }

    return page_action_vector;
  }
