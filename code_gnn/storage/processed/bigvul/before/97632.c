xmlXPathCompiledEvalToBoolean(xmlXPathCompExprPtr comp,
			      xmlXPathContextPtr ctxt)
{
    return(xmlXPathCompiledEvalInternal(comp, ctxt, NULL, 1));
}
