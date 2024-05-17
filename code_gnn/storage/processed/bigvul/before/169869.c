xsltInitElemPreComp(xsltElemPreCompPtr comp, xsltStylesheetPtr style,
                    xmlNodePtr inst, xsltTransformFunction function,
                    xsltElemPreCompDeallocator freeFunc)
{
    comp->type = XSLT_FUNC_EXTENSION;
    comp->func = function;
    comp->inst = inst;
    comp->free = freeFunc;

    comp->next = style->preComps;
    style->preComps = comp;
}
