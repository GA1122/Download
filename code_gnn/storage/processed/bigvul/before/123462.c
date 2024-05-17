ScriptPromise ImageBitmapFactories::createImageBitmap(EventTarget& eventTarget, ImageData* data, int sx, int sy, int sw, int sh, ExceptionState& exceptionState)
{
    if (!data) {
        exceptionState.throwTypeError("The ImageData provided is invalid.");
        return ScriptPromise();
    }
    if (!sw || !sh) {
        exceptionState.throwDOMException(IndexSizeError, String::format("The source %s provided is 0.", sw ? "height" : "width"));
        return ScriptPromise();
    }
    return fulfillImageBitmap(eventTarget.executionContext(), ImageBitmap::create(data, IntRect(sx, sy, sw, sh)));
}
