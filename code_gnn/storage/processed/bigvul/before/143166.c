void Document::currentScriptForBinding(HTMLScriptElementOrSVGScriptElement& scriptElement) const
{
    if (Element* script = currentScript()) {
        if (script->isInV1ShadowTree())
            return;
        if (isHTMLScriptElement(script))
            scriptElement.setHTMLScriptElement(toHTMLScriptElement(script));
        else if (isSVGScriptElement(script))
            scriptElement.setSVGScriptElement(toSVGScriptElement(script));
    }
}
