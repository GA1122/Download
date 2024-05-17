bool CastStreamingNativeHandler::IPEndPointFromArg(
    v8::Isolate* isolate,
    const v8::Local<v8::Value>& arg,
    net::IPEndPoint* ip_endpoint) const {
  scoped_ptr<V8ValueConverter> converter(V8ValueConverter::create());
  scoped_ptr<base::Value> destination_value(
      converter->FromV8Value(arg, context()->v8_context()));
  if (!destination_value) {
    isolate->ThrowException(v8::Exception::TypeError(
        v8::String::NewFromUtf8(isolate, kInvalidAesIvMask)));
    return false;
  }
  scoped_ptr<IPEndPoint> destination =
      IPEndPoint::FromValue(*destination_value);
  if (!destination) {
    isolate->ThrowException(v8::Exception::TypeError(
        v8::String::NewFromUtf8(isolate, kInvalidDestination)));
    return false;
  }
  net::IPAddress ip;
  if (!ip.AssignFromIPLiteral(destination->address)) {
    isolate->ThrowException(v8::Exception::TypeError(
        v8::String::NewFromUtf8(isolate, kInvalidDestination)));
    return false;
  }
  *ip_endpoint = net::IPEndPoint(ip, destination->port);
  return true;
}
