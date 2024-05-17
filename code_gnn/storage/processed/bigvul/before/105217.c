static PassRefPtr<EditingStyle> styleFromMatchedRulesAndInlineDecl(const Node* node)
{
    if (!node->isHTMLElement())
        return 0;

    HTMLElement* element = const_cast<HTMLElement*>(static_cast<const HTMLElement*>(node));
    RefPtr<EditingStyle> style = EditingStyle::create(element->inlineStyle());
    style->mergeStyleFromRules(element);
    return style.release();
}
