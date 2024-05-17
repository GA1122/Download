ScriptPromise ImageBitmapFactories::createImageBitmap(EventTarget& eventTarget, HTMLVideoElement* video, int sx, int sy, int sw, int sh, ExceptionState& exceptionState)
{
    ASSERT(eventTarget.toDOMWindow());

    if (!video) {
        exceptionState.throwTypeError("The video element provided is invalid.");
        return ScriptPromise();
    }
    if (!video->player()) {
        exceptionState.throwDOMException(InvalidStateError, "No player can be retrieved from the provided video element.");
        return ScriptPromise();
    }
    if (video->networkState() == HTMLMediaElement::NETWORK_EMPTY) {
        exceptionState.throwDOMException(InvalidStateError, "The provided element has not retrieved data.");
        return ScriptPromise();
    }
    if (video->player()->readyState() <= MediaPlayer::HaveMetadata) {
        exceptionState.throwDOMException(InvalidStateError, "The provided element's player has no current data.");
        return ScriptPromise();
    }
    if (!sw || !sh) {
        exceptionState.throwDOMException(IndexSizeError, String::format("The source %s provided is 0.", sw ? "height" : "width"));
        return ScriptPromise();
    }
    if (!video->hasSingleSecurityOrigin()) {
        exceptionState.throwSecurityError("The source video contains image data from multiple origins.");
        return ScriptPromise();
    }
    if (!video->player()->didPassCORSAccessCheck() && eventTarget.toDOMWindow()->document()->securityOrigin()->taintsCanvas(video->currentSrc())) {
        exceptionState.throwSecurityError("Cross-origin access to the source video is denied.");
        return ScriptPromise();
    }
    return fulfillImageBitmap(eventTarget.executionContext(), ImageBitmap::create(video, IntRect(sx, sy, sw, sh)));
}
