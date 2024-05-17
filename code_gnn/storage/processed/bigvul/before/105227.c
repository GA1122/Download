void HTMLElement::adjustDirectionalityIfNeededAfterChildAttributeChanged(Element* child)
{
    ASSERT(selfOrAncestorHasDirAutoAttribute());
    Node* strongDirectionalityTextNode;
    TextDirection textDirection = directionality(&strongDirectionalityTextNode);
    setHasDirAutoFlagRecursively(child, false);
    if (renderer() && renderer()->style() && renderer()->style()->direction() != textDirection) {
        Element* elementToAdjust = this;
        for (; elementToAdjust; elementToAdjust = elementToAdjust->parentElement()) {
            if (elementAffectsDirectionality(elementToAdjust)) {
                elementToAdjust->setNeedsStyleRecalc();
                return;
            }
        }
    }
}
