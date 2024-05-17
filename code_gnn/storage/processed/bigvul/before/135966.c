void ContainerNode::removeChildren(SubtreeModificationAction action)
{
    if (!m_firstChild)
        return;

    RefPtrWillBeRawPtr<ContainerNode> protect(this);

    willRemoveChildren();

    {
        SubframeLoadingDisabler disabler(*this);

        document().removeFocusedElementOfSubtree(this, true);

        document().nodeChildrenWillBeRemoved(*this);
    }

#if !ENABLE(OILPAN)
    NodeVector removedChildren;
#endif
    {
        HTMLFrameOwnerElement::UpdateSuspendScope suspendWidgetHierarchyUpdates;

        {
            EventDispatchForbiddenScope assertNoEventDispatch;
            ScriptForbiddenScope forbidScript;

#if !ENABLE(OILPAN)
            removedChildren.reserveInitialCapacity(countChildren());
#endif
            while (RefPtrWillBeRawPtr<Node> child = m_firstChild) {
                removeBetween(0, child->nextSibling(), *child);
#if !ENABLE(OILPAN)
                removedChildren.append(child.get());
#endif
                notifyNodeRemoved(*child);
            }
        }

        ChildrenChange change = {AllChildrenRemoved, nullptr, nullptr, ChildrenChangeSourceAPI};
        childrenChanged(change);
    }

    if (action == DispatchSubtreeModifiedEvent)
        dispatchSubtreeModifiedEvent();
}
