void Range::setStart(const Position& start, ExceptionCode& ec)
{
    Position parentAnchored = start.parentAnchoredEquivalent();
    setStart(parentAnchored.containerNode(), parentAnchored.offsetInContainerNode(), ec);
}
