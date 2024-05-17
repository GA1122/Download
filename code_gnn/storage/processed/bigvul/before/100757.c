xmlParserEntityCheck(xmlParserCtxtPtr ctxt, unsigned long size,
                     xmlEntityPtr ent)
{
    unsigned long consumed = 0;

    if ((ctxt == NULL) || (ctxt->options & XML_PARSE_HUGE))
        return (0);
    if (ctxt->lastError.code == XML_ERR_ENTITY_LOOP)
        return (1);
    if (size != 0) {
         
        if (size < XML_PARSER_BIG_ENTITY)
	    return(0);

         
        if (ctxt->input != NULL) {
            consumed = ctxt->input->consumed +
                (ctxt->input->cur - ctxt->input->base);
        }
        consumed += ctxt->sizeentities;

        if ((size < XML_PARSER_NON_LINEAR * consumed) &&
	    (ctxt->nbentities * 3 < XML_PARSER_NON_LINEAR * consumed))
            return (0);
    } else if (ent != NULL) {
         
        size = ent->checked;

         
        if (ctxt->input != NULL) {
            consumed = ctxt->input->consumed +
                (ctxt->input->cur - ctxt->input->base);
        }
        consumed += ctxt->sizeentities;

         
        if (size * 3 < consumed * XML_PARSER_NON_LINEAR)
            return (0);
    } else {
         
        return (0);
    }

    xmlFatalErr(ctxt, XML_ERR_ENTITY_LOOP, NULL);
    return (1);
}
