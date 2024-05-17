PassRefPtr<ShadowRoot> ShadowRoot::create(Element* element, ExceptionCode& ec)
{
    return create(element, CreatingAuthorShadowRoot, ec);
}
