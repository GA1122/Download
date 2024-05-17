ScriptPromise ImageBitmapFactories::createImageBitmap(EventTarget& eventTarget, CanvasRenderingContext2D* context, ExceptionState& exceptionState)
{
    return createImageBitmap(eventTarget, context->canvas(), exceptionState);
}
