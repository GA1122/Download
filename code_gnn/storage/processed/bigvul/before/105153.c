ShadowRoot* Range::shadowRoot() const
{
    return startContainer() ? startContainer()->shadowRoot() : 0;
}
