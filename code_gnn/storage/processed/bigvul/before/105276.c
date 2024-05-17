PassRefPtr<DocumentFragment> HTMLElement::textToFragment(const String& text, ExceptionCode& ec)
{
    RefPtr<DocumentFragment> fragment = DocumentFragment::create(document());
    unsigned int i, length = text.length();
    UChar c = 0;
    for (unsigned int start = 0; start < length; ) {

        for (i = start; i < length; i++) {
          c = text[i];
          if (c == '\r' || c == '\n')
              break;
        }

        fragment->appendChild(Text::create(document(), text.substring(start, i - start)), ec);
        if (ec)
            return 0;

        if (c == '\r' || c == '\n') {
            fragment->appendChild(HTMLBRElement::create(document()), ec);
            if (ec)
                return 0;
            if (c == '\r' && i + 1 < length && text[i + 1] == '\n')
                i++;
        }

        start = i + 1;  
    }

    return fragment;
}
