bool InspectorPageAgent::cachedResourceContent(Resource* cachedResource, String* result, bool* base64Encoded)
{
    bool hasZeroSize;
    bool prepared = prepareResourceBuffer(cachedResource, &hasZeroSize);
    if (!prepared)
        return false;

    *base64Encoded = !hasTextContent(cachedResource);
    if (*base64Encoded) {
        RefPtr<SharedBuffer> buffer = hasZeroSize ? SharedBuffer::create() : cachedResource->resourceBuffer();

        if (!buffer)
            return false;

        *result = base64Encode(buffer->data(), buffer->size());
        return true;
    }

    if (hasZeroSize) {
        *result = "";
        return true;
    }

    if (cachedResource) {
        switch (cachedResource->type()) {
        case Resource::CSSStyleSheet:
            *result = static_cast<CSSStyleSheetResource*>(cachedResource)->sheetText(false);
            return true;
        case Resource::Script:
            *result = static_cast<WebCore::ScriptResource*>(cachedResource)->script();
            return true;
        case Resource::MainResource:
            return false;
        case Resource::Raw: {
            SharedBuffer* buffer = cachedResource->resourceBuffer();
            if (!buffer)
                return false;
            RefPtr<TextResourceDecoder> decoder = createXHRTextDecoder(cachedResource->response().mimeType(), cachedResource->response().textEncodingName());
            String content = decoder->decode(buffer->data(), buffer->size());
            *result = content + decoder->flush();
            return true;
        }
        default:
            SharedBuffer* buffer = cachedResource->resourceBuffer();
            return decodeBuffer(buffer ? buffer->data() : 0, buffer ? buffer->size() : 0, cachedResource->response().textEncodingName(), result);
        }
    }
    return false;
}
