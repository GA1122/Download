void ContainerNode::parserAppendChild(PassRefPtr<Node> newChild)
{
    ASSERT(newChild);
    ASSERT(!newChild->parentNode());  
    ASSERT(!newChild->isDocumentFragment());
    ASSERT(!hasTagName(HTMLNames::templateTag));

    if (&document() != &newChild->document())
        document().adoptNode(newChild.get(), ASSERT_NO_EXCEPTION);

    Node* last = m_lastChild;
    {
        NoEventDispatchAssertion assertNoEventDispatch;
        appendChildToContainer(newChild.get(), this);
        treeScope().adoptIfNeeded(newChild.get());
    }

    newChild->updateAncestorConnectedSubframeCountForInsertion();

    ChildListMutationScope(this).childAdded(newChild.get());

    childrenChanged(true, last, 0, 1);
    ChildNodeInsertionNotifier(this).notify(newChild.get());
}
