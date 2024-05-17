ScriptPromise ImageBitmapFactories::createImageBitmap(EventTarget& eventTarget, Blob* blob, int sx, int sy, int sw, int sh, ExceptionState& exceptionState)
{
    if (!blob) {
        exceptionState.throwTypeError("The blob provided is invalid.");
        return ScriptPromise();
    }
    if (!sw || !sh) {
        exceptionState.throwDOMException(IndexSizeError, String::format("The source %s provided is 0.", sw ? "height" : "width"));
        return ScriptPromise();
    }
    RefPtr<ScriptPromiseResolver> resolver = ScriptPromiseResolver::create(eventTarget.executionContext());
    ScriptPromise promise = resolver->promise();
    RefPtr<ImageBitmapLoader> loader = ImageBitmapFactories::ImageBitmapLoader::create(from(eventTarget), resolver, IntRect(sx, sy, sw, sh));
    from(eventTarget).addLoader(loader);
    loader->loadBlobAsync(eventTarget.executionContext(), blob);
    return promise;
}
