void Document::pushCurrentScript(Element* newCurrentScript)
{
    DCHECK(isHTMLScriptElement(newCurrentScript) || isSVGScriptElement(newCurrentScript));
    m_currentScriptStack.append(newCurrentScript);
}
