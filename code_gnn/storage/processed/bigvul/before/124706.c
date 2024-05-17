void RenderBlockFlow::createFloatingObjects()
{
    m_floatingObjects = adoptPtr(new FloatingObjects(this, isHorizontalWritingMode()));
}
