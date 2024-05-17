static bool shouldIgnoreElement(const Element& element)
{
    return isHTMLScriptElement(element) || isHTMLNoScriptElement(element) || isCharsetSpecifyingNode(element);
}
