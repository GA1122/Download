  static v8::Handle<v8::Value> SetIconCommon(
      const v8::Arguments& args) {
    Value* bitmap_value = NULL;
    if (!ConvertImageDataToBitmapValue(args, &bitmap_value))
      return v8::Undefined();

    v8::Local<v8::Object> extension_args = args[1]->ToObject();
    v8::Local<v8::Object> details =
        extension_args->Get(v8::String::New("0"))->ToObject();

    DictionaryValue* dict = new DictionaryValue();
    dict->Set("imageData", bitmap_value);

    if (details->Has(v8::String::New("tabId"))) {
      dict->SetInteger("tabId",
                       details->Get(v8::String::New("tabId"))->Int32Value());
    }

    ListValue list_value;
    list_value.Append(dict);

    return StartRequestCommon(args, &list_value);
  }
