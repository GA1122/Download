exsltCryptoCryptoApiReportError (xmlXPathParserContextPtr ctxt,
				 int line) {
    LPVOID lpMsgBuf;
    DWORD dw = GetLastError ();

    FormatMessage (FORMAT_MESSAGE_ALLOCATE_BUFFER |
		   FORMAT_MESSAGE_FROM_SYSTEM, NULL, dw,
		   MAKELANGID (LANG_NEUTRAL, SUBLANG_DEFAULT),
		   (LPTSTR) & lpMsgBuf, 0, NULL);

    xsltTransformError (xsltXPathGetTransformContext (ctxt), NULL, NULL,
			"exslt:crypto error (line %d). %s", line,
			lpMsgBuf);
    LocalFree (lpMsgBuf);
}
