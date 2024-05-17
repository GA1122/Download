 ScriptPromise ImageBitmapFactories::createImageBitmap(EventTarget& eventTarget, Blob* blob, ExceptionState& exceptionState)
{
    if (!blob) {
        exceptionState.throwTypeError("The blob provided is invalid.");
        return ScriptPromise();
    }
    RefPtr<ScriptPromiseResolver> resolver = ScriptPromiseResolver::create(eventTarget.executionContext());
    ScriptPromise promise = resolver->promise();
    RefPtr<ImageBitmapLoader> loader = ImageBitmapFactories::ImageBitmapLoader::create(from(eventTarget), resolver, IntRect());
    from(eventTarget).addLoader(loader);
    loader->loadBlobAsync(eventTarget.executionContext(), blob);
    return promise;
}
