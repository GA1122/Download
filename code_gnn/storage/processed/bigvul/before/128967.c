    bool readArrayBufferView(v8::Handle<v8::Value>* value, CompositeCreator& creator)
    {
        ArrayBufferViewSubTag subTag;
        uint32_t byteOffset;
        uint32_t byteLength;
        RefPtr<ArrayBuffer> arrayBuffer;
        v8::Handle<v8::Value> arrayBufferV8Value;
        if (!readArrayBufferViewSubTag(&subTag))
            return false;
        if (!doReadUint32(&byteOffset))
            return false;
        if (!doReadUint32(&byteLength))
            return false;
        if (!creator.consumeTopOfStack(&arrayBufferV8Value))
            return false;
        if (arrayBufferV8Value.IsEmpty())
            return false;
        arrayBuffer = V8ArrayBuffer::toNative(arrayBufferV8Value.As<v8::Object>());
        if (!arrayBuffer)
            return false;

        v8::Handle<v8::Object> creationContext = m_scriptState->context()->Global();
        switch (subTag) {
        case ByteArrayTag:
            *value = toV8(Int8Array::create(arrayBuffer.release(), byteOffset, byteLength), creationContext, isolate());
            break;
        case UnsignedByteArrayTag:
            *value = toV8(Uint8Array::create(arrayBuffer.release(), byteOffset, byteLength), creationContext,  isolate());
            break;
        case UnsignedByteClampedArrayTag:
            *value = toV8(Uint8ClampedArray::create(arrayBuffer.release(), byteOffset, byteLength), creationContext, isolate());
            break;
        case ShortArrayTag: {
            uint32_t shortLength = byteLength / sizeof(int16_t);
            if (shortLength * sizeof(int16_t) != byteLength)
                return false;
            *value = toV8(Int16Array::create(arrayBuffer.release(), byteOffset, shortLength), creationContext, isolate());
            break;
        }
        case UnsignedShortArrayTag: {
            uint32_t shortLength = byteLength / sizeof(uint16_t);
            if (shortLength * sizeof(uint16_t) != byteLength)
                return false;
            *value = toV8(Uint16Array::create(arrayBuffer.release(), byteOffset, shortLength), creationContext, isolate());
            break;
        }
        case IntArrayTag: {
            uint32_t intLength = byteLength / sizeof(int32_t);
            if (intLength * sizeof(int32_t) != byteLength)
                return false;
            *value = toV8(Int32Array::create(arrayBuffer.release(), byteOffset, intLength), creationContext, isolate());
            break;
        }
        case UnsignedIntArrayTag: {
            uint32_t intLength = byteLength / sizeof(uint32_t);
            if (intLength * sizeof(uint32_t) != byteLength)
                return false;
            *value = toV8(Uint32Array::create(arrayBuffer.release(), byteOffset, intLength), creationContext, isolate());
            break;
        }
        case FloatArrayTag: {
            uint32_t floatLength = byteLength / sizeof(float);
            if (floatLength * sizeof(float) != byteLength)
                return false;
            *value = toV8(Float32Array::create(arrayBuffer.release(), byteOffset, floatLength), creationContext, isolate());
            break;
        }
        case DoubleArrayTag: {
            uint32_t floatLength = byteLength / sizeof(double);
            if (floatLength * sizeof(double) != byteLength)
                return false;
            *value = toV8(Float64Array::create(arrayBuffer.release(), byteOffset, floatLength), creationContext, isolate());
            break;
        }
        case DataViewTag:
            *value = toV8(DataView::create(arrayBuffer.release(), byteOffset, byteLength), creationContext, isolate());
            break;
        default:
            return false;
        }
        return !value->IsEmpty();
    }
