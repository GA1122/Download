void LayoutBlockFlow::invalidatePaintForOverhangingFloats(bool paintAllDescendants)
{
    if (!hasOverhangingFloats())
        return;

    const FloatingObjectSet& floatingObjectSet = m_floatingObjects->set();
    FloatingObjectSetIterator end = floatingObjectSet.end();
    for (FloatingObjectSetIterator it = floatingObjectSet.begin(); it != end; ++it) {
        const FloatingObject& floatingObject = *it->get();
        if (logicalBottomForFloat(floatingObject) > logicalHeight()
            && !floatingObject.layoutObject()->hasSelfPaintingLayer()
            && (floatingObject.isDirectlyContained() || (paintAllDescendants && floatingObject.isDescendant()))) {

            LayoutBox* floatingLayoutBox = floatingObject.layoutObject();
            floatingLayoutBox->setShouldDoFullPaintInvalidation();
            floatingLayoutBox->invalidatePaintForOverhangingFloats(false);
        }
    }
}
