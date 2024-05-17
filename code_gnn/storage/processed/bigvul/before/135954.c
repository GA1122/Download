void ContainerNode::notifyNodeInserted(Node& root, ChildrenChangeSource source)
{
    ASSERT(!EventDispatchForbiddenScope::isEventDispatchForbidden());
    ASSERT(!root.isShadowRoot());

    InspectorInstrumentation::didInsertDOMNode(&root);

    RefPtrWillBeRawPtr<Node> protect(this);
    RefPtrWillBeRawPtr<Node> protectNode(root);

    NodeVector postInsertionNotificationTargets;
    notifyNodeInsertedInternal(root, postInsertionNotificationTargets);

    childrenChanged(ChildrenChange::forInsertion(root, source));

    for (const auto& targetNode : postInsertionNotificationTargets) {
        if (targetNode->inDocument())
            targetNode->didNotifySubtreeInsertionsToDocument();
    }
}
