  static v8::Handle<v8::Value> GetUniqueSubEventName(
      const v8::Arguments& args) {
    static int next_event_id = 0;
    DCHECK(args.Length() == 1);
    DCHECK(args[0]->IsString());
    std::string event_name(*v8::String::AsciiValue(args[0]));
    std::string unique_event_name =
        event_name + "/" + base::IntToString(++next_event_id);
    return v8::String::New(unique_event_name.c_str());
  }
