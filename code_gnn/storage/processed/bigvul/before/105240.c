TextDirection HTMLElement::directionality(Node** strongDirectionalityTextNode) const
{
    if (HTMLTextFormControlElement* textElement = toTextFormControl(const_cast<HTMLElement*>(this))) {
        bool hasStrongDirectionality;
        Unicode::Direction textDirection = textElement->value().defaultWritingDirection(&hasStrongDirectionality);
        if (strongDirectionalityTextNode)
            *strongDirectionalityTextNode = hasStrongDirectionality ? textElement : 0;
        return (textDirection == Unicode::LeftToRight) ? LTR : RTL;
    }

    Node* node = firstChild();
    while (node) {
        if (equalIgnoringCase(node->nodeName(), "bdi") || node->hasTagName(scriptTag) || node->hasTagName(styleTag) 
            || (node->isElementNode() && toElement(node)->isTextFormControl())) {
            node = node->traverseNextSibling(this);
            continue;
        }

        if (node->isElementNode()) {
            AtomicString dirAttributeValue = toElement(node)->fastGetAttribute(dirAttr);
            if (equalIgnoringCase(dirAttributeValue, "rtl") || equalIgnoringCase(dirAttributeValue, "ltr") || equalIgnoringCase(dirAttributeValue, "auto")) {
                node = node->traverseNextSibling(this);
                continue;
            }
        }

        if (node->isTextNode()) {
            bool hasStrongDirectionality;
            WTF::Unicode::Direction textDirection = node->textContent(true).defaultWritingDirection(&hasStrongDirectionality);
            if (hasStrongDirectionality) {
                if (strongDirectionalityTextNode)
                    *strongDirectionalityTextNode = node;
                return (textDirection == WTF::Unicode::LeftToRight) ? LTR : RTL;
            }
        }
        node = node->traverseNextNode(this);
    }
    if (strongDirectionalityTextNode)
        *strongDirectionalityTextNode = 0;
    return LTR;
}
