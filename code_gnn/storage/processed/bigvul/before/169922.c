xsltFreeNamespaceAliasHashes(xsltStylesheetPtr style) {
    if (style->nsAliases != NULL)
	xmlHashFree((xmlHashTablePtr) style->nsAliases, NULL);
    style->nsAliases = NULL;
}
