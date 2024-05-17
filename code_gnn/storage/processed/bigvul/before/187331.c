 xsltNumberFormatGetMultipleLevel(xsltTransformContextPtr context,
 				 xmlNodePtr node,
  				 xsltCompMatchPtr countPat,
  				 xsltCompMatchPtr fromPat,
  				 double *array,
				 int max,
				 xmlDocPtr doc,
				 xmlNodePtr elem)
// 				 int max)
  {
      int amount = 0;
      int cnt;
     xmlNodePtr ancestor;
     xmlNodePtr preceding;
     xmlXPathParserContextPtr parser;
 
     context->xpathCtxt->node = node;
     parser = xmlXPathNewParserContext(NULL, context->xpathCtxt);
     if (parser) {
 	 
 	for (ancestor = node;
 	     (ancestor != NULL) && (ancestor->type != XML_DOCUMENT_NODE);
 	     ancestor = xmlXPathNextAncestor(parser, ancestor)) {
 
 	    if ((fromPat != NULL) &&
  		xsltTestCompMatchList(context, ancestor, fromPat))
  		break;  
  
	    if ((countPat == NULL && node->type == ancestor->type &&
		xmlStrEqual(node->name, ancestor->name)) ||
		xsltTestCompMatchList(context, ancestor, countPat)) {
// 	    if (xsltTestCompMatchCount(context, ancestor, countPat, node)) {
  		 
		cnt = 0;
		for (preceding = ancestor;
// 		cnt = 1;
// 		for (preceding =
//                         xmlXPathNextPrecedingSibling(parser, ancestor);
  		     preceding != NULL;
  		     preceding =
  		        xmlXPathNextPrecedingSibling(parser, preceding)) {
		    if (countPat == NULL) {
			if ((preceding->type == ancestor->type) &&
			    xmlStrEqual(preceding->name, ancestor->name)){
			    if ((preceding->ns == ancestor->ns) ||
			        ((preceding->ns != NULL) &&
				 (ancestor->ns != NULL) &&
			         (xmlStrEqual(preceding->ns->href,
			             ancestor->ns->href) )))
			        cnt++;
			}
		    } else {
			if (xsltTestCompMatchList(context, preceding,
				                  countPat))
			    cnt++;
		    }
// 
// 	            if (xsltTestCompMatchCount(context, preceding, countPat,
//                                                node))
// 			cnt++;
  		}
  		array[amount++] = (double)cnt;
  		if (amount >= max)
 		    break;  
 	    }
 	}
 	xmlXPathFreeParserContext(parser);
     }
     return amount;
 }