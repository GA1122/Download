void ContainerNode::parserAddChild(PassRefPtr<Node> newChild)
{
    ASSERT(newChild);
    ASSERT(!newChild->parentNode());  

    forbidEventDispatch();
    Node* last = m_lastChild;
    appendChildToContainer(newChild.get(), this);
    treeScope()->adoptIfNeeded(newChild.get());
    
    allowEventDispatch();

    childrenChanged(true, last, 0, 1);
    ChildNodeInsertionNotifier(this).notify(newChild.get());
}
