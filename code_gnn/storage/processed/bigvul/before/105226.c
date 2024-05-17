void HTMLElement::addHTMLLengthToStyle(StylePropertySet* style, CSSPropertyID propertyID, const String& value)
{

    StringImpl* v = value.impl();
    if (v) {
        unsigned int l = 0;

        while (l < v->length() && (*v)[l] <= ' ')
            l++;

        for (; l < v->length(); l++) {
            UChar cc = (*v)[l];
            if (cc > '9')
                break;
            if (cc < '0') {
                if (cc == '%' || cc == '*')
                    l++;
                if (cc != '.')
                    break;
            }
        }

        if (l != v->length()) {
            addPropertyToAttributeStyle(style, propertyID, v->substring(0, l));
            return;
        }
    }

    addPropertyToAttributeStyle(style, propertyID, value);
}
