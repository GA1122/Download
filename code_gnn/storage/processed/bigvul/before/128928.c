Serializer::StateBase* Serializer::doSerialize(v8::Handle<v8::Value> value, StateBase* next)
{
    m_writer.writeReferenceCount(m_nextObjectReference);
    uint32_t objectReference;
    uint32_t arrayBufferIndex;
    if ((value->IsObject() || value->IsDate() || value->IsRegExp())
        && m_objectPool.tryGet(value.As<v8::Object>(), &objectReference)) {
        ASSERT(!value->IsString());
        m_writer.writeObjectReference(objectReference);
    } else if (value.IsEmpty()) {
        return handleError(InputError, "The empty property name cannot be cloned.", next);
    } else if (value->IsUndefined()) {
        m_writer.writeUndefined();
    } else if (value->IsNull()) {
        m_writer.writeNull();
    } else if (value->IsTrue()) {
        m_writer.writeTrue();
    } else if (value->IsFalse()) {
        m_writer.writeFalse();
    } else if (value->IsInt32()) {
        m_writer.writeInt32(value->Int32Value());
    } else if (value->IsUint32()) {
        m_writer.writeUint32(value->Uint32Value());
    } else if (value->IsNumber()) {
        m_writer.writeNumber(value.As<v8::Number>()->Value());
    } else if (V8ArrayBufferView::hasInstance(value, isolate())) {
        return writeAndGreyArrayBufferView(value.As<v8::Object>(), next);
    } else if (value->IsString()) {
        writeString(value);
    } else if (V8MessagePort::hasInstance(value, isolate())) {
        uint32_t messagePortIndex;
        if (m_transferredMessagePorts.tryGet(value.As<v8::Object>(), &messagePortIndex)) {
            m_writer.writeTransferredMessagePort(messagePortIndex);
        } else {
            return handleError(DataCloneError, "A MessagePort could not be cloned.", next);
        }
    } else if (V8ArrayBuffer::hasInstance(value, isolate()) && m_transferredArrayBuffers.tryGet(value.As<v8::Object>(), &arrayBufferIndex)) {
        return writeTransferredArrayBuffer(value, arrayBufferIndex, next);
    } else {
        v8::Handle<v8::Object> jsObject = value.As<v8::Object>();
        if (jsObject.IsEmpty())
            return handleError(DataCloneError, "An object could not be cloned.", next);
        greyObject(jsObject);
        if (value->IsDate()) {
            m_writer.writeDate(value->NumberValue());
        } else if (value->IsStringObject()) {
            writeStringObject(value);
        } else if (value->IsNumberObject()) {
            writeNumberObject(value);
        } else if (value->IsBooleanObject()) {
            writeBooleanObject(value);
        } else if (value->IsArray()) {
            return startArrayState(value.As<v8::Array>(), next);
        } else if (V8File::hasInstance(value, isolate())) {
            return writeFile(value, next);
        } else if (V8Blob::hasInstance(value, isolate())) {
            return writeBlob(value, next);
        } else if (V8DOMFileSystem::hasInstance(value, isolate())) {
            return writeDOMFileSystem(value, next);
        } else if (V8FileList::hasInstance(value, isolate())) {
            return writeFileList(value, next);
        } else if (V8CryptoKey::hasInstance(value, isolate())) {
            if (!writeCryptoKey(value))
                return handleError(DataCloneError, "Couldn't serialize key data", next);
        } else if (V8ImageData::hasInstance(value, isolate())) {
            writeImageData(value);
        } else if (value->IsRegExp()) {
            writeRegExp(value);
        } else if (V8ArrayBuffer::hasInstance(value, isolate())) {
            return writeArrayBuffer(value, next);
        } else if (value->IsObject()) {
            if (isHostObject(jsObject) || jsObject->IsCallable() || value->IsNativeError())
                return handleError(DataCloneError, "An object could not be cloned.", next);
            return startObjectState(jsObject, next);
        } else {
            return handleError(DataCloneError, "A value could not be cloned.", next);
        }
    }
    return 0;
}
