int Document::requestAnimationFrame(FrameRequestCallback* callback)
{
    return ensureScriptedAnimationController().registerCallback(callback);
}
