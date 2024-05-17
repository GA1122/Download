bool isResponseEmpty(PassRefPtr<TypeBuilder::Network::Response> response)
{
    if (!response)
        return true;

    RefPtr<JSONValue> status = response->get("status");
    RefPtr<JSONValue> mimeType = response->get("mimeType");
    RefPtr<JSONObject> headers = response->getObject("headers");

    return !status && !mimeType && (!headers || !headers->size());
}
