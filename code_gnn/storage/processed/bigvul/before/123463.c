ScriptPromise ImageBitmapFactories::createImageBitmap(EventTarget& eventTarget, ImageBitmap* bitmap, ExceptionState& exceptionState)
{
    return createImageBitmap(eventTarget, bitmap, 0, 0, bitmap->width(), bitmap->height(), exceptionState);
}
