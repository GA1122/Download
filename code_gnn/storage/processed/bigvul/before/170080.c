xsltLREEffectiveNsNodesFree(xsltEffectiveNsPtr first)
{
    xsltEffectiveNsPtr tmp;

    while (first != NULL) {
	tmp = first;
	first = first->nextInStore;
	xmlFree(tmp);
    }
}
