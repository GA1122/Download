exsltStrRegister (void) {
    xsltRegisterExtModuleFunction ((const xmlChar *) "tokenize",
				   EXSLT_STRINGS_NAMESPACE,
				   exsltStrTokenizeFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "split",
				   EXSLT_STRINGS_NAMESPACE,
				   exsltStrSplitFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "encode-uri",
				   EXSLT_STRINGS_NAMESPACE,
				   exsltStrEncodeUriFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "decode-uri",
				   EXSLT_STRINGS_NAMESPACE,
				   exsltStrDecodeUriFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "padding",
				   EXSLT_STRINGS_NAMESPACE,
				   exsltStrPaddingFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "align",
				   EXSLT_STRINGS_NAMESPACE,
				   exsltStrAlignFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "concat",
				   EXSLT_STRINGS_NAMESPACE,
				   exsltStrConcatFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "replace",
				   EXSLT_STRINGS_NAMESPACE,
				   exsltStrReplaceFunction);
}
