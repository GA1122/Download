void SerializedScriptValue::transferImageBitmaps(v8::Isolate* isolate, const ImageBitmapArray& imageBitmaps, ExceptionState& exceptionState)
{
    if (!imageBitmaps.size())
        return;

    for (size_t i = 0; i < imageBitmaps.size(); ++i) {
        if (imageBitmaps[i]->isNeutered()) {
            exceptionState.throwDOMException(DataCloneError, "ImageBitmap at index " + String::number(i) + " is already neutered.");
            return;
        }
    }

    OwnPtr<ImageBitmapContentsArray> contents = adoptPtr(new ImageBitmapContentsArray);
    HeapHashSet<Member<ImageBitmap>> visited;
    for (size_t i = 0; i < imageBitmaps.size(); ++i) {
        if (visited.contains(imageBitmaps[i]))
            continue;
        visited.add(imageBitmaps[i]);
        contents->append(imageBitmaps[i]->transfer());
    }
    m_imageBitmapContentsArray = contents.release();
}
