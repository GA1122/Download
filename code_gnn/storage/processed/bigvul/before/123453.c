ScriptPromise ImageBitmapFactories::createImageBitmap(EventTarget& eventTarget, HTMLImageElement* image, int sx, int sy, int sw, int sh, ExceptionState& exceptionState)
{
    ASSERT(eventTarget.toDOMWindow());

    if (!image) {
        exceptionState.throwTypeError("The image element provided is invalid.");
        return ScriptPromise();
    }
    if (!image->cachedImage()) {
        exceptionState.throwDOMException(InvalidStateError, "No image can be retrieved from the provided element.");
        return ScriptPromise();
    }
    if (image->cachedImage()->image()->isSVGImage()) {
        exceptionState.throwDOMException(InvalidStateError, "The image element contains an SVG image, which is unsupported.");
        return ScriptPromise();
    }
    if (!sw || !sh) {
        exceptionState.throwDOMException(IndexSizeError, String::format("The source %s provided is 0.", sw ? "height" : "width"));
        return ScriptPromise();
    }
    if (!image->cachedImage()->image()->currentFrameHasSingleSecurityOrigin()) {
        exceptionState.throwSecurityError("The source image contains image data from multiple origins.");
        return ScriptPromise();
    }
    if (!image->cachedImage()->passesAccessControlCheck(eventTarget.toDOMWindow()->document()->securityOrigin()) && eventTarget.toDOMWindow()->document()->securityOrigin()->taintsCanvas(image->src())) {
        exceptionState.throwSecurityError("Cross-origin access to the source image is denied.");
        return ScriptPromise();
    }
    return fulfillImageBitmap(eventTarget.executionContext(), ImageBitmap::create(image, IntRect(sx, sy, sw, sh)));
}
