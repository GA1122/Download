RenderFlexibleBox::RenderFlexibleBox(Element* element)
    : RenderBlock(element)
    , m_orderIterator(this)
    , m_numberOfInFlowChildrenOnFirstLine(-1)
{
    setChildrenInline(false);  
}
