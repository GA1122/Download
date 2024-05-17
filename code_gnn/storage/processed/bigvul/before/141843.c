static void dumpV8Message(v8::Local<v8::Context> context, v8::Local<v8::Message> message)
{
    if (message.IsEmpty())
        return;

    message->GetScriptOrigin();
    v8::Maybe<int> unused = message->GetLineNumber(context);
    ALLOW_UNUSED_LOCAL(unused);

    v8::Local<v8::Value> resourceName = message->GetScriptOrigin().ResourceName();
    String fileName = "Unknown JavaScript file";
    if (!resourceName.IsEmpty() && resourceName->IsString())
        fileName = toCoreString(v8::Local<v8::String>::Cast(resourceName));
    int lineNumber = 0;
    v8Call(message->GetLineNumber(context), lineNumber);
    v8::Local<v8::String> errorMessage = message->Get();
    fprintf(stderr, "%s (line %d): %s\n", fileName.utf8().data(), lineNumber, toCoreString(errorMessage).utf8().data());
}
