void LayoutBlockFlow::deleteLineBoxTree()
{
    if (containsFloats())
        m_floatingObjects->clearLineBoxTreePointers();

    m_lineBoxes.deleteLineBoxTree();
}
