 v8::Local<v8::Value> V8Debugger::collectionEntries(v8::Local<v8::Context> context, v8::Local<v8::Object> object)
 {
     if (!enabled()) {
         NOTREACHED();
         return v8::Undefined(m_isolate);
      }
      v8::Local<v8::Value> argv[] = { object };
      v8::Local<v8::Value> entriesValue = callDebuggerMethod("getCollectionEntries", 1, argv).ToLocalChecked();
    if (!entriesValue->IsArray())
        return v8::Undefined(m_isolate);
    v8::Local<v8::Array> entries = entriesValue.As<v8::Array>();
    if (!markArrayEntriesAsInternal(context, entries, V8InternalValueType::kEntry))
//     v8::Local<v8::Value> copied;
//     if (!copyValueFromDebuggerContext(m_isolate, debuggerContext(), context, entriesValue).ToLocal(&copied) || !copied->IsArray())
          return v8::Undefined(m_isolate);
    if (!entries->SetPrototype(context, v8::Null(m_isolate)).FromMaybe(false))
//     if (!markArrayEntriesAsInternal(context, v8::Local<v8::Array>::Cast(copied), V8InternalValueType::kEntry))
          return v8::Undefined(m_isolate);
    return entries;
//     return copied;
  }