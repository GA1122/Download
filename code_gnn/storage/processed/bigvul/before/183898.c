  void HTMLElement::setInnerHTML(const String& html, ExceptionCode& ec)
  {
    RefPtr<DocumentFragment> fragment = createFragmentFromSource(html, this, ec);
    if (fragment)
//     if (RefPtr<DocumentFragment> fragment = createFragmentForInnerOuterHTML(html, this, ec))
          replaceChildrenWithFragment(this, fragment.release(), ec);
  }