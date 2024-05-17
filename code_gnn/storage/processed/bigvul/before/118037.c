static v8::Handle<v8::Value> toV8Object(const WebGLGetInfo& info, v8::Isolate* isolate)
{
    switch (info.getType()) {
    case WebGLGetInfo::kTypeBool:
        return v8::Boolean::New(info.getBool());
    case WebGLGetInfo::kTypeBoolArray: {
        const Vector<bool>& value = info.getBoolArray();
        v8::Local<v8::Array> array = v8::Array::New(value.size());
        for (size_t ii = 0; ii < value.size(); ++ii)
            array->Set(v8::Integer::New(ii), v8::Boolean::New(value[ii]));
        return array;
    }
    case WebGLGetInfo::kTypeFloat:
        return v8::Number::New(info.getFloat());
    case WebGLGetInfo::kTypeInt:
        return v8::Integer::New(info.getInt());
    case WebGLGetInfo::kTypeNull:
        return v8::Null();
    case WebGLGetInfo::kTypeString:
        return v8::String::New(fromWebCoreString(info.getString()), info.getString().length());
    case WebGLGetInfo::kTypeUnsignedInt:
        return v8::Integer::NewFromUnsigned(info.getUnsignedInt());
    case WebGLGetInfo::kTypeWebGLBuffer:
        return toV8(info.getWebGLBuffer(), isolate);
    case WebGLGetInfo::kTypeWebGLFloatArray:
        return toV8(info.getWebGLFloatArray(), isolate);
    case WebGLGetInfo::kTypeWebGLFramebuffer:
        return toV8(info.getWebGLFramebuffer(), isolate);
    case WebGLGetInfo::kTypeWebGLIntArray:
        return toV8(info.getWebGLIntArray(), isolate);
    case WebGLGetInfo::kTypeWebGLProgram:
        return toV8(info.getWebGLProgram(), isolate);
    case WebGLGetInfo::kTypeWebGLRenderbuffer:
        return toV8(info.getWebGLRenderbuffer(), isolate);
    case WebGLGetInfo::kTypeWebGLTexture:
        return toV8(info.getWebGLTexture(), isolate);
    case WebGLGetInfo::kTypeWebGLUnsignedByteArray:
        return toV8(info.getWebGLUnsignedByteArray(), isolate);
    case WebGLGetInfo::kTypeWebGLUnsignedIntArray:
        return toV8(info.getWebGLUnsignedIntArray(), isolate);
    case WebGLGetInfo::kTypeWebGLVertexArrayObjectOES:
        return toV8(info.getWebGLVertexArrayObjectOES(), isolate);
    default:
        notImplemented();
        return v8::Undefined();
    }
}
