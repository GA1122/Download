bool V8Proxy::registeredExtensionWithV8(v8::Extension* extension)
{
    const V8Extensions& registeredExtensions = extensions();
    for (size_t i = 0; i < registeredExtensions.size(); ++i) {
        if (registeredExtensions[i] == extension)
            return true;
    }

    return false;
}
