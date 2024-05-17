int V8Debugger::contextId(v8::Local<v8::Context> context)
{
    v8::Local<v8::Value> data = context->GetEmbedderData(static_cast<int>(v8::Context::kDebugIdIndex));
    if (data.IsEmpty() || !data->IsString())
        return 0;
    String16 dataString = toProtocolString(data.As<v8::String>());
    if (dataString.isEmpty())
        return 0;
    size_t commaPos = dataString.find(",");
    if (commaPos == String16::kNotFound)
        return 0;
    size_t commaPos2 = dataString.find(",", commaPos + 1);
    if (commaPos2 == String16::kNotFound)
        return 0;
    return dataString.substring(commaPos + 1, commaPos2 - commaPos - 1).toInteger();
}
