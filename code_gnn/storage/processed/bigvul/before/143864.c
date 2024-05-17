NodeIterator::NodePointer::NodePointer(PassRefPtrWillBeRawPtr<Node> n, bool b)
    : node(n)
    , isPointerBeforeNode(b)
{
}
