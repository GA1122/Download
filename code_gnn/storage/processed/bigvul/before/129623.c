bool SVGLayoutSupport::isLayoutableTextNode(const LayoutObject* object)
{
    ASSERT(object->isText());
    return object->isSVGInlineText() && !toLayoutSVGInlineText(object)->hasEmptyText();
}
