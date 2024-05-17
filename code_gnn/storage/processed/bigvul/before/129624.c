bool SVGLayoutSupport::isOverflowHidden(const LayoutObject* object)
{
    ASSERT(!object->isDocumentElement());

    return object->style()->overflowX() == OverflowHidden || object->style()->overflowX() == OverflowScroll;
}
