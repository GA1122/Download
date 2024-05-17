xsltRunStylesheet(xsltStylesheetPtr style, xmlDocPtr doc,
                  const char **params, const char *output,
                  xmlSAXHandlerPtr SAX, xmlOutputBufferPtr IObuf)
{
    return(xsltRunStylesheetUser(style, doc, params, output, SAX, IObuf,
		                 NULL, NULL));
}
