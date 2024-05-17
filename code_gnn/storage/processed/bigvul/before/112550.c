void Document::setBody(PassRefPtr<HTMLElement> prpNewBody, ExceptionCode& ec)
{
    RefPtr<HTMLElement> newBody = prpNewBody;

    if (!newBody || !documentElement() || !newBody->hasTagName(bodyTag)) { 
        ec = HIERARCHY_REQUEST_ERR;
        return;
    }

    if (newBody->document() && newBody->document() != this) {
        ec = 0;
        RefPtr<Node> node = importNode(newBody.get(), true, ec);
        if (ec)
            return;
        
        newBody = toHTMLElement(node.get());
    }

    HTMLElement* b = body();
    if (!b)
        documentElement()->appendChild(newBody.release(), ec);
    else
        documentElement()->replaceChild(newBody.release(), b, ec);
}
