void ContainerNode::parserAppendChild(PassRefPtrWillBeRawPtr<Node> newChild)
{
    ASSERT(newChild);
    ASSERT(!newChild->isDocumentFragment());
    ASSERT(!isHTMLTemplateElement(this));

    if (!checkParserAcceptChild(*newChild))
        return;

    RefPtrWillBeRawPtr<Node> protect(this);

    while (RefPtrWillBeRawPtr<ContainerNode> parent = newChild->parentNode())
        parent->parserRemoveChild(*newChild);

    if (document() != newChild->document())
        document().adoptNode(newChild.get(), ASSERT_NO_EXCEPTION);

    {
        EventDispatchForbiddenScope assertNoEventDispatch;
        ScriptForbiddenScope forbidScript;

        treeScope().adoptIfNeeded(*newChild);
        appendChildCommon(*newChild);
        newChild->updateAncestorConnectedSubframeCountForInsertion();
        ChildListMutationScope(*this).childAdded(*newChild);
    }

    notifyNodeInserted(*newChild, ChildrenChangeSourceParser);
}
