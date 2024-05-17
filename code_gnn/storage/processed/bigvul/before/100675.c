namePush(xmlParserCtxtPtr ctxt, const xmlChar * value)
{
    if (ctxt == NULL) return (-1);

    if (ctxt->nameNr >= ctxt->nameMax) {
        const xmlChar * *tmp;
        ctxt->nameMax *= 2;
        tmp = (const xmlChar * *) xmlRealloc((xmlChar * *)ctxt->nameTab,
                                    ctxt->nameMax *
                                    sizeof(ctxt->nameTab[0]));
        if (tmp == NULL) {
	    ctxt->nameMax /= 2;
	    goto mem_error;
        }
	ctxt->nameTab = tmp;
    }
    ctxt->nameTab[ctxt->nameNr] = value;
    ctxt->name = value;
    return (ctxt->nameNr++);
mem_error:
    xmlErrMemory(ctxt, NULL);
    return (-1);
}
