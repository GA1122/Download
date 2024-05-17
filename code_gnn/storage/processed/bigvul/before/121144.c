void HTMLInputElement::didAddUserAgentShadowRoot(ShadowRoot*)
{
    m_inputType->createShadowSubtree();
}
