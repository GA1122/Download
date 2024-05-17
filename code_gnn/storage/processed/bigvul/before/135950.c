PassRefPtrWillBeRawPtr<Node> ContainerNode::insertBefore(PassRefPtrWillBeRawPtr<Node> newChild, Node* refChild, ExceptionState& exceptionState)
{
#if !ENABLE(OILPAN)
    ASSERT(refCount() || parentOrShadowHostNode());
#endif

    RefPtrWillBeRawPtr<Node> protect(this);

    if (!refChild) {
        return appendChild(newChild, exceptionState);
    }

    if (!checkAcceptChild(newChild.get(), 0, exceptionState)) {
        if (exceptionState.hadException())
            return nullptr;
        return newChild;
    }
    ASSERT(newChild);

    if (refChild->parentNode() != this) {
        exceptionState.throwDOMException(NotFoundError, "The node before which the new node is to be inserted is not a child of this node.");
        return nullptr;
    }

    if (refChild->previousSibling() == newChild || refChild == newChild)
        return newChild;

    RefPtrWillBeRawPtr<Node> next = refChild;

    NodeVector targets;
    collectChildrenAndRemoveFromOldParent(*newChild, targets, exceptionState);
    if (exceptionState.hadException())
        return nullptr;
    if (targets.isEmpty())
        return newChild;

    if (!checkAcceptChildGuaranteedNodeTypes(*newChild, nullptr, exceptionState)) {
        if (exceptionState.hadException())
            return nullptr;
        return newChild;
    }

    InspectorInstrumentation::willInsertDOMNode(this);

    ChildListMutationScope mutation(*this);
    for (const auto& targetNode : targets) {
        ASSERT(targetNode);
        Node& child = *targetNode;

        if (next->parentNode() != this)
            break;
        if (child.parentNode())
            break;

        {
            EventDispatchForbiddenScope assertNoEventDispatch;
            ScriptForbiddenScope forbidScript;

            treeScope().adoptIfNeeded(child);
            insertBeforeCommon(*next, child);
        }

        updateTreeAfterInsertion(child);
    }

    dispatchSubtreeModifiedEvent();

    return newChild;
}
