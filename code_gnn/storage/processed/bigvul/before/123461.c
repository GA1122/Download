ScriptPromise ImageBitmapFactories::createImageBitmap(EventTarget& eventTarget, ImageData* data, ExceptionState& exceptionState)
{
    return createImageBitmap(eventTarget, data, 0, 0, data->width(), data->height(), exceptionState);
}
