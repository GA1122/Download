bool InspectorPageAgent::sharedBufferContent(PassRefPtr<SharedBuffer> buffer, const String& textEncodingName, bool withBase64Encode, String* result)
{
    return dataContent(buffer ? buffer->data() : 0, buffer ? buffer->size() : 0, textEncodingName, withBase64Encode, result);
}
