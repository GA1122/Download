void ContainerNode::appendChild(PassRefPtr<Node> newChild, ExceptionState& es)
{
    RefPtr<ContainerNode> protect(this);

    ASSERT(refCount() || parentOrShadowHostNode());

    if (!checkAddChild(this, newChild.get(), es))
        return;

    if (newChild == m_lastChild)  
        return;

    NodeVector targets;
    collectChildrenAndRemoveFromOldParent(newChild.get(), targets, es);
    if (es.hadException())
        return;

    if (targets.isEmpty())
        return;

    if (!checkAcceptChildGuaranteedNodeTypes(this, newChild.get(), es))
        return;

    InspectorInstrumentation::willInsertDOMNode(&document(), this);

    ChildListMutationScope mutation(this);
    for (NodeVector::const_iterator it = targets.begin(); it != targets.end(); ++it) {
        Node* child = it->get();

        if (child->parentNode())
            break;

        treeScope().adoptIfNeeded(child);

        {
            NoEventDispatchAssertion assertNoEventDispatch;
            appendChildToContainer(child, this);
        }

        updateTreeAfterInsertion(this, child);
    }

    dispatchSubtreeModifiedEvent();
}
