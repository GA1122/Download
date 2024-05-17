ScriptPromise ImageBitmapFactories::createImageBitmap(EventTarget& eventTarget, HTMLVideoElement* video, ExceptionState& exceptionState)
{
    IntSize s = sizeFor(video);
    return createImageBitmap(eventTarget, video, 0, 0, s.width(), s.height(), exceptionState);
}
