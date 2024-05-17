RenderObject* RenderBox::splitAnonymousBoxesAroundChild(RenderObject* beforeChild)
{
    bool didSplitParentAnonymousBoxes = false;

    while (beforeChild->parent() != this) {
        RenderBox* boxToSplit = toRenderBox(beforeChild->parent());
        if (boxToSplit->firstChild() != beforeChild && boxToSplit->isAnonymous()) {
            didSplitParentAnonymousBoxes = true;

            RenderBox* postBox = boxToSplit->createAnonymousBoxWithSameTypeAs(this);
            postBox->setChildrenInline(boxToSplit->childrenInline());
            RenderBox* parentBox = toRenderBox(boxToSplit->parent());
            markBoxForRelayoutAfterSplit(parentBox);
            parentBox->virtualChildren()->insertChildNode(parentBox, postBox, boxToSplit->nextSibling());
            boxToSplit->moveChildrenTo(postBox, beforeChild, 0, true);

            markBoxForRelayoutAfterSplit(boxToSplit);
            markBoxForRelayoutAfterSplit(postBox);

            beforeChild = postBox;
        } else
            beforeChild = boxToSplit;
    }

    if (didSplitParentAnonymousBoxes)
        markBoxForRelayoutAfterSplit(this);

    ASSERT(beforeChild->parent() == this);
    return beforeChild;
}
