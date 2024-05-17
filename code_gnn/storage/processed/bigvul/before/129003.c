PassOwnPtr<SerializedScriptValue::ArrayBufferContentsArray> SerializedScriptValue::transferArrayBuffers(ArrayBufferArray& arrayBuffers, ExceptionState& exceptionState, v8::Isolate* isolate)
{
    ASSERT(arrayBuffers.size());

    for (size_t i = 0; i < arrayBuffers.size(); i++) {
        if (arrayBuffers[i]->isNeutered()) {
            exceptionState.throwDOMException(DataCloneError, "ArrayBuffer at index " + String::number(i) + " is already neutered.");
            return nullptr;
        }
    }

    OwnPtr<ArrayBufferContentsArray> contents = adoptPtr(new ArrayBufferContentsArray(arrayBuffers.size()));

    HashSet<ArrayBuffer*> visited;
    for (size_t i = 0; i < arrayBuffers.size(); i++) {
        if (visited.contains(arrayBuffers[i].get()))
            continue;
        visited.add(arrayBuffers[i].get());

        bool result = arrayBuffers[i]->transfer(contents->at(i));
        if (!result) {
            exceptionState.throwDOMException(DataCloneError, "ArrayBuffer at index " + String::number(i) + " could not be transferred.");
            return nullptr;
        }

        neuterArrayBufferInAllWorlds(arrayBuffers[i].get());
    }
    return contents.release();
}
