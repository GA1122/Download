RenderObject* HTMLInputElement::createRenderer(RenderArena* arena, RenderStyle* style)
{
    return m_inputType->createRenderer(arena, style);
}
