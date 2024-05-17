bool RenderBlockFlow::hasOverhangingFloat(RenderBox* renderer)
{
    if (!m_floatingObjects || hasColumns() || !parent())
        return false;

    const FloatingObjectSet& floatingObjectSet = m_floatingObjects->set();
    FloatingObjectSetIterator it = floatingObjectSet.find<FloatingObjectHashTranslator>(renderer);
    if (it == floatingObjectSet.end())
        return false;

    return logicalBottomForFloat(*it) > logicalHeight();
}
