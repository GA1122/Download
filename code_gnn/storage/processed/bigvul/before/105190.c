void StyledMarkupAccumulator::appendText(StringBuilder& out, Text* text)
{    
    const bool parentIsTextarea = text->parentElement() && text->parentElement()->tagQName() == textareaTag;
    const bool wrappingSpan = shouldApplyWrappingStyle(text) && !parentIsTextarea;
    if (wrappingSpan) {
        RefPtr<EditingStyle> wrappingStyle = m_wrappingStyle->copy();
        wrappingStyle->forceInline();
        wrappingStyle->style()->setProperty(CSSPropertyFloat, CSSValueNone);

        StringBuilder openTag;
        appendStyleNodeOpenTag(openTag, wrappingStyle->style(), text->document());
        out.append(openTag.characters(), openTag.length());
    }

    if (!shouldAnnotate() || parentIsTextarea)
        MarkupAccumulator::appendText(out, text);
    else {
        const bool useRenderedText = !enclosingNodeWithTag(firstPositionInNode(text), selectTag);
        String content = useRenderedText ? renderedText(text, m_range) : stringValueForRange(text, m_range);
        StringBuilder buffer;
        appendCharactersReplacingEntities(buffer, content.characters(), content.length(), EntityMaskInPCDATA);
        out.append(convertHTMLTextToInterchangeFormat(buffer.toString(), text));
    }

    if (wrappingSpan)
        out.append(styleNodeCloseTag());
}
