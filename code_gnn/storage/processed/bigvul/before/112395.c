HTMLElement* Document::body() const
{
    Node* de = documentElement();
    if (!de)
        return 0;
    
    Node* body = 0;
    for (Node* i = de->firstChild(); i; i = i->nextSibling()) {
        if (i->hasTagName(framesetTag))
            return toHTMLElement(i);
        
        if (i->hasTagName(bodyTag) && !body)
            body = i;
    }
    return toHTMLElement(body);
}
