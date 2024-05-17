void Document::setBody(PassRefPtr<HTMLElement> prpNewBody, ExceptionState& es)
{
    RefPtr<HTMLElement> newBody = prpNewBody;

    if (!newBody || !documentElement()) {
        es.throwUninformativeAndGenericDOMException(HierarchyRequestError);
        return;
    }

    if (!newBody->hasTagName(bodyTag) && !newBody->hasTagName(framesetTag)) {
        es.throwUninformativeAndGenericDOMException(HierarchyRequestError);
        return;
    }

    HTMLElement* oldBody = body();
    if (oldBody == newBody)
        return;

    if (oldBody)
        documentElement()->replaceChild(newBody.release(), oldBody, es);
    else
        documentElement()->appendChild(newBody.release(), es);
}
