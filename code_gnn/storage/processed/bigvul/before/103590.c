static void willRemoveChild(Node* child)
{
#if ENABLE(MUTATION_OBSERVERS)
    ASSERT(child->parentNode());
    ChildListMutationScope(child->parentNode()).willRemoveChild(child);
    child->notifyMutationObserversNodeWillDetach();
#endif

    dispatchChildRemovalEvents(child);
    child->document()->nodeWillBeRemoved(child);  
    ChildFrameDisconnector(child).disconnect();
}