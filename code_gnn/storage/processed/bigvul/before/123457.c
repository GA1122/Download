ScriptPromise ImageBitmapFactories::createImageBitmap(EventTarget& eventTarget, CanvasRenderingContext2D* context, int sx, int sy, int sw, int sh, ExceptionState& exceptionState)
{
    return createImageBitmap(eventTarget, context->canvas(), sx, sy, sw, sh, exceptionState);
}
