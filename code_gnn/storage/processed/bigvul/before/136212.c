void PageSerializer::serializeCSSStyleSheet(CSSStyleSheet& styleSheet, const KURL& url)
{
    StringBuilder cssText;
    for (unsigned i = 0; i < styleSheet.length(); ++i) {
        CSSRule* rule = styleSheet.item(i);
        String itemText = rule->cssText();
        if (!itemText.isEmpty()) {
            cssText.append(itemText);
            if (i < styleSheet.length() - 1)
                cssText.appendLiteral("\n\n");
        }

        serializeCSSRule(rule);
    }

    if (url.isValid() && !m_resourceURLs.contains(url)) {
        WTF::TextEncoding textEncoding(styleSheet.contents()->charset());
        ASSERT(textEncoding.isValid());
        String textString = cssText.toString();
        CString text = textEncoding.normalizeAndEncode(textString, WTF::EntitiesForUnencodables);
        m_resources->append(SerializedResource(url, String("text/css"), SharedBuffer::create(text.data(), text.length())));
        m_resourceURLs.add(url);
    }
}
