void V8Proxy::registerExtensionWithV8(v8::Extension* extension)
{
    if (!registeredExtensionWithV8(extension))
        v8::RegisterExtension(extension);
}
