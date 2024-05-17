void Document::enqueueMediaQueryChangeListeners(HeapVector<Member<MediaQueryListListener>>& listeners)
{
    ensureScriptedAnimationController().enqueueMediaQueryChangeListeners(listeners);
}
