bool RenderBlockFlow::containsFloat(RenderBox* renderer) const
{
    return m_floatingObjects && m_floatingObjects->set().contains<FloatingObjectHashTranslator>(renderer);
}
