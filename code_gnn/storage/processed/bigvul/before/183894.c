  void ShadowRoot::setInnerHTML(const String& markup, ExceptionCode& ec)
  {
    RefPtr<DocumentFragment> fragment = createFragmentFromSource(markup, host(), ec);
    if (fragment)
//     if (RefPtr<DocumentFragment> fragment = createFragmentForInnerOuterHTML(markup, host(), ec))
          replaceChildrenWithFragment(this, fragment.release(), ec);
  }