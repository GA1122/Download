PassOwnPtr<v8::ScriptData> V8Proxy::precompileScript(v8::Handle<v8::String> code, CachedScript* cachedScript)
{
    static const unsigned dataTypeID = 0xECC13BD7;

    static const int minPreparseLength = 1024;

    if (!cachedScript || code->Length() < minPreparseLength)
        return nullptr;

    CachedMetadata* cachedMetadata = cachedScript->cachedMetadata(dataTypeID);
    if (cachedMetadata)
        return adoptPtr(v8::ScriptData::New(cachedMetadata->data(), cachedMetadata->size()));

    OwnPtr<v8::ScriptData> scriptData = adoptPtr(v8::ScriptData::PreCompile(code));
    cachedScript->setCachedMetadata(dataTypeID, scriptData->Data(), scriptData->Length());

    return scriptData.release();
}
