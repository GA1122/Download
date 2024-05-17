RenderBox::RenderBox(ContainerNode* node)
    : RenderBoxModelObject(node)
    , m_intrinsicContentLogicalHeight(-1)
    , m_minPreferredLogicalWidth(-1)
    , m_maxPreferredLogicalWidth(-1)
{
    setIsBox();
}
