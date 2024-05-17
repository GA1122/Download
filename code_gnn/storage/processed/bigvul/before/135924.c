PassRefPtrWillBeRawPtr<Node> ContainerNode::appendChild(PassRefPtrWillBeRawPtr<Node> newChild, ExceptionState& exceptionState)
{
    RefPtrWillBeRawPtr<ContainerNode> protect(this);

#if !ENABLE(OILPAN)
    ASSERT(refCount() || parentOrShadowHostNode());
#endif

    if (!checkAcceptChild(newChild.get(), 0, exceptionState)) {
        if (exceptionState.hadException())
            return nullptr;
        return newChild;
    }
    ASSERT(newChild);

    if (newChild == m_lastChild)  
        return newChild;

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

        if (child.parentNode())
            break;

        {
            EventDispatchForbiddenScope assertNoEventDispatch;
            ScriptForbiddenScope forbidScript;

            treeScope().adoptIfNeeded(child);
            appendChildCommon(child);
        }

        updateTreeAfterInsertion(child);
    }

    dispatchSubtreeModifiedEvent();
    return newChild;
}
