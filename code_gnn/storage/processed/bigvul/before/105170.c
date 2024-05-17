void ShadowRoot::attach()
{
    StyleResolver* styleResolver = document()->styleResolver();
    styleResolver->pushParentShadowRoot(this);
    DocumentFragment::attach();
    styleResolver->popParentShadowRoot(this);
}
