xmlXPathCompiledEval(xmlXPathCompExprPtr comp, xmlXPathContextPtr ctx)
{
    xmlXPathObjectPtr res = NULL;

    xmlXPathCompiledEvalInternal(comp, ctx, &res, 0);
    return(res);
}
