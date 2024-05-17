static v8::Handle<v8::Value> toV8Object(WebGLExtension* extension, v8::Handle<v8::Object> contextObject, v8::Isolate* isolate)
{
    if (!extension)
        return v8::Null();
    v8::Handle<v8::Value> extensionObject;
    const char* referenceName = 0;
    switch (extension->getName()) {
    case WebGLExtension::WebKitWebGLLoseContextName:
        extensionObject = toV8(static_cast<WebGLLoseContext*>(extension), isolate);
        referenceName = "webKitWebGLLoseContextName";
        break;
    case WebGLExtension::EXTTextureFilterAnisotropicName:
        extensionObject = toV8(static_cast<EXTTextureFilterAnisotropic*>(extension), isolate);
        referenceName = "extTextureFilterAnisotropicName";
        break;
    case WebGLExtension::OESStandardDerivativesName:
        extensionObject = toV8(static_cast<OESStandardDerivatives*>(extension), isolate);
        referenceName = "oesStandardDerivativesName";
        break;
    case WebGLExtension::OESTextureFloatName:
        extensionObject = toV8(static_cast<OESTextureFloat*>(extension), isolate);
        referenceName = "oesTextureFloatName";
        break;
    case WebGLExtension::OESVertexArrayObjectName:
        extensionObject = toV8(static_cast<OESVertexArrayObject*>(extension), isolate);
        referenceName = "oesVertexArrayObjectName";
        break;
    case WebGLExtension::WebGLDebugRendererInfoName:
        extensionObject = toV8(static_cast<WebGLDebugRendererInfo*>(extension), isolate);
        referenceName = "webGLDebugRendererInfoName";
        break;
    case WebGLExtension::WebGLDebugShadersName:
        extensionObject = toV8(static_cast<WebGLDebugShaders*>(extension), isolate);
        referenceName = "webGLDebugShadersName";
        break;
    case WebGLExtension::WebKitWebGLCompressedTextureS3TCName:
        extensionObject = toV8(static_cast<WebGLCompressedTextureS3TC*>(extension), isolate);
        referenceName = "webKitWebGLCompressedTextureS3TCName";
        break;
    }
    ASSERT(!extensionObject.IsEmpty());
    V8DOMWrapper::setNamedHiddenReference(contextObject, referenceName, extensionObject);
    return extensionObject;
}
