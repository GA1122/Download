bool SerializedScriptValue::extractTransferables(v8::Isolate* isolate, v8::Local<v8::Value> value, int argumentIndex, Transferables& transferables, ExceptionState& exceptionState)
{
    if (isUndefinedOrNull(value))
        return true;

    uint32_t length = 0;
    if (value->IsArray()) {
        v8::Local<v8::Array> array = v8::Local<v8::Array>::Cast(value);
        length = array->Length();
    } else if (!toV8Sequence(value, length, isolate, exceptionState)) {
        if (!exceptionState.hadException())
            exceptionState.throwTypeError(ExceptionMessages::notAnArrayTypeArgumentOrValue(argumentIndex + 1));
        return false;
    }

    v8::Local<v8::Object> transferableArray = v8::Local<v8::Object>::Cast(value);

    for (unsigned i = 0; i < length; ++i) {
        v8::Local<v8::Value> transferableObject;
        if (!transferableArray->Get(isolate->GetCurrentContext(), i).ToLocal(&transferableObject))
            return false;
        if (isUndefinedOrNull(transferableObject)) {
            exceptionState.throwTypeError("Value at index " + String::number(i) + " is an untransferable " + (transferableObject->IsUndefined() ? "'undefined'" : "'null'") + " value.");
            return false;
        }
        if (V8MessagePort::hasInstance(transferableObject, isolate)) {
            MessagePort* port = V8MessagePort::toImpl(v8::Local<v8::Object>::Cast(transferableObject));
            if (transferables.messagePorts.contains(port)) {
                exceptionState.throwDOMException(DataCloneError, "Message port at index " + String::number(i) + " is a duplicate of an earlier port.");
                return false;
            }
            transferables.messagePorts.append(port);
        } else if (V8ArrayBuffer::hasInstance(transferableObject, isolate)) {
            DOMArrayBuffer* arrayBuffer = V8ArrayBuffer::toImpl(v8::Local<v8::Object>::Cast(transferableObject));
            if (transferables.arrayBuffers.contains(arrayBuffer)) {
                exceptionState.throwDOMException(DataCloneError, "ArrayBuffer at index " + String::number(i) + " is a duplicate of an earlier ArrayBuffer.");
                return false;
            }
            transferables.arrayBuffers.append(arrayBuffer);
        } else if (V8SharedArrayBuffer::hasInstance(transferableObject, isolate)) {
            DOMSharedArrayBuffer* sharedArrayBuffer = V8SharedArrayBuffer::toImpl(v8::Local<v8::Object>::Cast(transferableObject));
            if (transferables.arrayBuffers.contains(sharedArrayBuffer)) {
                exceptionState.throwDOMException(DataCloneError, "SharedArrayBuffer at index " + String::number(i) + " is a duplicate of an earlier SharedArrayBuffer.");
                return false;
            }
            transferables.arrayBuffers.append(sharedArrayBuffer);
        } else if (V8ImageBitmap::hasInstance(transferableObject, isolate)) {
            ImageBitmap* imageBitmap = V8ImageBitmap::toImpl(v8::Local<v8::Object>::Cast(transferableObject));
            if (transferables.imageBitmaps.contains(imageBitmap)) {
                exceptionState.throwDOMException(DataCloneError, "ImageBitmap at index " + String::number(i) + " is a duplicate of an earlier ImageBitmap.");
                return false;
            }
            transferables.imageBitmaps.append(imageBitmap);
        } else {
            exceptionState.throwTypeError("Value at index " + String::number(i) + " does not have a transferable type.");
            return false;
        }
    }
    return true;
}
