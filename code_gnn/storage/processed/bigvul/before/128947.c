bool SerializedScriptValue::extractTransferables(v8::Local<v8::Value> value, int argumentIndex, MessagePortArray& ports, ArrayBufferArray& arrayBuffers, ExceptionState& exceptionState, v8::Isolate* isolate)
{
    if (isUndefinedOrNull(value)) {
        ports.resize(0);
        arrayBuffers.resize(0);
        return true;
    }

    uint32_t length = 0;
    if (value->IsArray()) {
        v8::Local<v8::Array> array = v8::Local<v8::Array>::Cast(value);
        length = array->Length();
    } else if (toV8Sequence(value, length, isolate).IsEmpty()) {
        exceptionState.throwTypeError(ExceptionMessages::notAnArrayTypeArgumentOrValue(argumentIndex + 1));
        return false;
    }

    v8::Local<v8::Object> transferrables = v8::Local<v8::Object>::Cast(value);

    for (unsigned i = 0; i < length; ++i) {
        v8::Local<v8::Value> transferrable = transferrables->Get(i);
        if (isUndefinedOrNull(transferrable)) {
            exceptionState.throwDOMException(DataCloneError, "Value at index " + String::number(i) + " is an untransferable " + (transferrable->IsUndefined() ? "'undefined'" : "'null'") + " value.");
            return false;
        }
        if (V8MessagePort::hasInstance(transferrable, isolate)) {
            RefPtrWillBeRawPtr<MessagePort> port = V8MessagePort::toNative(v8::Handle<v8::Object>::Cast(transferrable));
            if (ports.contains(port)) {
                exceptionState.throwDOMException(DataCloneError, "Message port at index " + String::number(i) + " is a duplicate of an earlier port.");
                return false;
            }
            ports.append(port.release());
        } else if (V8ArrayBuffer::hasInstance(transferrable, isolate)) {
            RefPtr<ArrayBuffer> arrayBuffer = V8ArrayBuffer::toNative(v8::Handle<v8::Object>::Cast(transferrable));
            if (arrayBuffers.contains(arrayBuffer)) {
                exceptionState.throwDOMException(DataCloneError, "ArrayBuffer at index " + String::number(i) + " is a duplicate of an earlier ArrayBuffer.");
                return false;
            }
            arrayBuffers.append(arrayBuffer.release());
        } else {
            exceptionState.throwDOMException(DataCloneError, "Value at index " + String::number(i) + " does not have a transferable type.");
            return false;
        }
    }
    return true;
}
