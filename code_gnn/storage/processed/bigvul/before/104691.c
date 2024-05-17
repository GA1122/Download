void ExtensionProcessBindings::HandleResponse(int request_id, bool success,
                                              const std::string& response,
                                              const std::string& error) {
  PendingRequestMap& pending_requests = GetPendingRequestMap();
  PendingRequestMap::iterator request = pending_requests.find(request_id);
  if (request == pending_requests.end())
    return;   

  v8::HandleScope handle_scope;
  v8::Handle<v8::Value> argv[5];
  argv[0] = v8::Integer::New(request_id);
  argv[1] = v8::String::New(request->second->name.c_str());
  argv[2] = v8::Boolean::New(success);
  argv[3] = v8::String::New(response.c_str());
  argv[4] = v8::String::New(error.c_str());
  v8::Handle<v8::Value> retval = bindings_utils::CallFunctionInContext(
      request->second->context, "handleResponse", arraysize(argv), argv);
#ifndef NDEBUG
  if (!retval.IsEmpty() && !retval->IsUndefined()) {
    std::string error = *v8::String::AsciiValue(retval);
    DCHECK(false) << error;
  }
#endif

  request->second->context.Dispose();
  request->second->context.Clear();
  pending_requests.erase(request);
}
