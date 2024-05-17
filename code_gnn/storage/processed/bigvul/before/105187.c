void StyledMarkupAccumulator::appendElement(StringBuilder& out, Element* element, bool addDisplayInline, RangeFullySelectsNode rangeFullySelectsNode)
{
    const bool documentIsHTML = element->document()->isHTMLDocument();
    appendOpenTag(out, element, 0);

    const unsigned length = element->hasAttributes() ? element->attributeCount() : 0;
    const bool shouldAnnotateOrForceInline = element->isHTMLElement() && (shouldAnnotate() || addDisplayInline);
    const bool shouldOverrideStyleAttr = shouldAnnotateOrForceInline || shouldApplyWrappingStyle(element);
    for (unsigned int i = 0; i < length; i++) {
        Attribute* attribute = element->attributeItem(i);
        if (attribute->name() == styleAttr && shouldOverrideStyleAttr)
            continue;
        appendAttribute(out, element, *attribute, 0);
    }

    if (shouldOverrideStyleAttr) {
        RefPtr<EditingStyle> newInlineStyle;

        if (shouldApplyWrappingStyle(element)) {
            newInlineStyle = m_wrappingStyle->copy();
            newInlineStyle->removePropertiesInElementDefaultStyle(element);
            newInlineStyle->removeStyleConflictingWithStyleOfNode(element);
        } else
            newInlineStyle = EditingStyle::create();

        if (element->isStyledElement() && static_cast<StyledElement*>(element)->inlineStyle())
            newInlineStyle->overrideWithStyle(static_cast<StyledElement*>(element)->inlineStyle());

        if (shouldAnnotateOrForceInline) {
            if (shouldAnnotate())
                newInlineStyle->mergeStyleFromRulesForSerialization(toHTMLElement(element));

            if (addDisplayInline)
                newInlineStyle->forceInline();

            if (rangeFullySelectsNode == DoesNotFullySelectNode && newInlineStyle->style())
                newInlineStyle->style()->removeProperty(CSSPropertyFloat);
        }

        if (!newInlineStyle->isEmpty()) {
            DEFINE_STATIC_LOCAL(const String, stylePrefix, (" style=\""));
            out.append(stylePrefix);
            appendAttributeValue(out, newInlineStyle->style()->asText(), documentIsHTML);
            out.append('\"');
        }
    }

    appendCloseTag(out, element);
}
