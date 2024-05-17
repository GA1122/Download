v8::Handle<v8::Value> V8WebSocket::closeCallback(const v8::Arguments& args)
{
    WebSocket* webSocket = toNative(args.Holder());
    int argumentCount = args.Length();
    int code = WebSocketChannel::CloseEventCodeNotSpecified;
    String reason = "";
    if (argumentCount >= 1) {
        double x = args[0]->NumberValue();
        double maxValue = static_cast<double>(std::numeric_limits<uint16_t>::max());
        double minValue = static_cast<double>(std::numeric_limits<uint16_t>::min());
        if (isnan(x))
            x = 0.0;
        else
            x = clampTo(x, minValue, maxValue);
        code = clampToInteger(x);
        if (argumentCount >= 2) {
            v8::TryCatch tryCatch;
            v8::Handle<v8::String> reasonValue = args[1]->ToString();
            if (tryCatch.HasCaught())
                return throwError(tryCatch.Exception(), args.GetIsolate());
            reason = toWebCoreString(reasonValue);
        }
    }
    ExceptionCode ec = 0;
    webSocket->close(code, reason, ec);
    if (ec)
        return throwError(ec, args.GetIsolate());
    return v8::Undefined();
}
