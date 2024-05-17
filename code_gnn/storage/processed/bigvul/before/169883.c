xsltRegisterTestModule(void)
{
    xsltInitGlobals();
    xsltRegisterExtModuleFull((const xmlChar *) XSLT_DEFAULT_URL,
                              xsltExtInitTest, xsltExtShutdownTest,
                              xsltExtStyleInitTest,
                              xsltExtStyleShutdownTest);
    xsltRegisterExtModuleFunction((const xmlChar *) "test",
                                  (const xmlChar *) XSLT_DEFAULT_URL,
                                  xsltExtFunctionTest);
    xsltRegisterExtModuleElement((const xmlChar *) "test",
                                 (const xmlChar *) XSLT_DEFAULT_URL,
                                 xsltExtElementPreCompTest,
                                 xsltExtElementTest);
}
