xsltFreeStylesheetList(xsltStylesheetPtr style) {
    xsltStylesheetPtr next;

    while (style != NULL) {
	next = style->next;
	xsltFreeStylesheet(style);
	style = next;
    }
}
