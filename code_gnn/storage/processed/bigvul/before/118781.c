static void willRemoveChild(Node* child)
{
    ASSERT(child->parentNode());
    ChildListMutationScope(child->parentNode()).willRemoveChild(child);
    child->notifyMutationObserversNodeWillDetach();
    dispatchChildRemovalEvents(child);
    child->document().nodeWillBeRemoved(child);  
    ChildFrameDisconnector(child).disconnect();
}
