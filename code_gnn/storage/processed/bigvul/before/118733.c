HTMLBodyElement* HTMLDocument::htmlBodyElement() const
{
    HTMLElement* body = this->body();
    return (body && body->hasTagName(bodyTag)) ? toHTMLBodyElement(body) : 0;
}
