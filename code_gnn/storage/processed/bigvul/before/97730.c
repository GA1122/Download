xmlXPathNodeCollectAndTest(xmlXPathParserContextPtr ctxt,
                           xmlXPathStepOpPtr op,
			   xmlNodePtr * first, xmlNodePtr * last,
			   int toBool)
{

#define XP_TEST_HIT \
    if (hasAxisRange != 0) { \
	if (++pos == maxPos) { \
	    addNode(seq, cur); \
	goto axis_range_end; } \
    } else { \
	addNode(seq, cur); \
	if (breakOnFirstHit) goto first_hit; }

#define XP_TEST_HIT_NS \
    if (hasAxisRange != 0) { \
	if (++pos == maxPos) { \
	    hasNsNodes = 1; \
	    xmlXPathNodeSetAddNs(seq, xpctxt->node, (xmlNsPtr) cur); \
	goto axis_range_end; } \
    } else { \
	hasNsNodes = 1; \
	xmlXPathNodeSetAddNs(seq, \
	xpctxt->node, (xmlNsPtr) cur); \
	if (breakOnFirstHit) goto first_hit; }

    xmlXPathAxisVal axis = (xmlXPathAxisVal) op->value;
    xmlXPathTestVal test = (xmlXPathTestVal) op->value2;
    xmlXPathTypeVal type = (xmlXPathTypeVal) op->value3;
    const xmlChar *prefix = op->value4;
    const xmlChar *name = op->value5;
    const xmlChar *URI = NULL;

#ifdef DEBUG_STEP
    int nbMatches = 0, prevMatches = 0;
#endif
    int total = 0, hasNsNodes = 0;
     
    xmlXPathObjectPtr obj;
     
    xmlNodeSetPtr contextSeq;
    int contextIdx;
    xmlNodePtr contextNode;
     
    xmlNodePtr outerContextNode;
     
    xmlNodeSetPtr outSeq;
     
    xmlNodeSetPtr seq;
    xmlNodePtr cur;
     
    xmlXPathStepOpPtr predOp;
    int maxPos;  
    int hasPredicateRange, hasAxisRange, pos, size, newSize;
    int breakOnFirstHit;

    xmlXPathTraversalFunction next = NULL;
     
    xmlXPathTraversalFunctionExt outerNext = NULL;
    void (*addNode) (xmlNodeSetPtr, xmlNodePtr);
    xmlXPathNodeSetMergeFunction mergeAndClear;
    xmlNodePtr oldContextNode;
    xmlXPathContextPtr xpctxt = ctxt->context;


    CHECK_TYPE0(XPATH_NODESET);
    obj = valuePop(ctxt);
     
    if (prefix != NULL) {
        URI = xmlXPathNsLookup(xpctxt, prefix);
        if (URI == NULL) {
	    xmlXPathReleaseObject(xpctxt, obj);
            XP_ERROR0(XPATH_UNDEF_PREFIX_ERROR);
	}
    }
     
    mergeAndClear = xmlXPathNodeSetMergeAndClear;
    switch (axis) {
        case AXIS_ANCESTOR:
            first = NULL;
            next = xmlXPathNextAncestor;
            break;
        case AXIS_ANCESTOR_OR_SELF:
            first = NULL;
            next = xmlXPathNextAncestorOrSelf;
            break;
        case AXIS_ATTRIBUTE:
            first = NULL;
	    last = NULL;
            next = xmlXPathNextAttribute;
	    mergeAndClear = xmlXPathNodeSetMergeAndClearNoDupls;
            break;
        case AXIS_CHILD:
	    last = NULL;
	    if (op->rewriteType == XP_REWRITE_DOS_CHILD_ELEM) {
		 
		outerNext = xmlXPathNextDescendantOrSelfElemParent;
	    }
	    if (((test == NODE_TEST_NAME) || (test == NODE_TEST_ALL)) &&
		(type == NODE_TYPE_NODE))
	    {
		 
		next = xmlXPathNextChildElement;
	    } else
		next = xmlXPathNextChild;
	    mergeAndClear = xmlXPathNodeSetMergeAndClearNoDupls;
            break;
        case AXIS_DESCENDANT:
	    last = NULL;
            next = xmlXPathNextDescendant;
            break;
        case AXIS_DESCENDANT_OR_SELF:
	    last = NULL;
            next = xmlXPathNextDescendantOrSelf;
            break;
        case AXIS_FOLLOWING:
	    last = NULL;
            next = xmlXPathNextFollowing;
            break;
        case AXIS_FOLLOWING_SIBLING:
	    last = NULL;
            next = xmlXPathNextFollowingSibling;
            break;
        case AXIS_NAMESPACE:
            first = NULL;
	    last = NULL;
            next = (xmlXPathTraversalFunction) xmlXPathNextNamespace;
	    mergeAndClear = xmlXPathNodeSetMergeAndClearNoDupls;
            break;
        case AXIS_PARENT:
            first = NULL;
            next = xmlXPathNextParent;
            break;
        case AXIS_PRECEDING:
            first = NULL;
            next = xmlXPathNextPrecedingInternal;
            break;
        case AXIS_PRECEDING_SIBLING:
            first = NULL;
            next = xmlXPathNextPrecedingSibling;
            break;
        case AXIS_SELF:
            first = NULL;
	    last = NULL;
            next = xmlXPathNextSelf;
	    mergeAndClear = xmlXPathNodeSetMergeAndClearNoDupls;
            break;
    }

#ifdef DEBUG_STEP
    xmlXPathDebugDumpStepAxis(op,
	(obj->nodesetval != NULL) ? obj->nodesetval->nodeNr : 0);
#endif

    if (next == NULL) {
	xmlXPathReleaseObject(xpctxt, obj);
        return(0);
    }
    contextSeq = obj->nodesetval;
    if ((contextSeq == NULL) || (contextSeq->nodeNr <= 0)) {
	xmlXPathReleaseObject(xpctxt, obj);
        valuePush(ctxt, xmlXPathCacheWrapNodeSet(xpctxt, NULL));
        return(0);
    }
     
    maxPos = 0;
    predOp = NULL;
    hasPredicateRange = 0;
    hasAxisRange = 0;
    if (op->ch2 != -1) {
	 
	predOp = &ctxt->comp->steps[op->ch2];
	if (xmlXPathIsPositionalPredicate(ctxt, predOp, &maxPos)) {
	    if (predOp->ch1 != -1) {
		 
		predOp = &ctxt->comp->steps[predOp->ch1];
		hasPredicateRange = 1;
	    } else {
		 
		predOp = NULL;
		hasAxisRange = 1;
	    }
	}
    }
    breakOnFirstHit = ((toBool) && (predOp == NULL)) ? 1 : 0;
     
     
    oldContextNode = xpctxt->node;
    addNode = xmlXPathNodeSetAddUnique;
    outSeq = NULL;
    seq = NULL;
    outerContextNode = NULL;
    contextNode = NULL;
    contextIdx = 0;


    while ((contextIdx < contextSeq->nodeNr) || (contextNode != NULL)) {
	if (outerNext != NULL) {
	     
	    if (contextNode == NULL) {
		 
		outerContextNode = contextSeq->nodeTab[contextIdx++];
		contextNode = outerNext(NULL, outerContextNode);
	    } else
		contextNode = outerNext(contextNode, outerContextNode);
	    if (contextNode == NULL)
		continue;
	     
	    xpctxt->node = contextNode;
	} else
	    xpctxt->node = contextSeq->nodeTab[contextIdx++];

	if (seq == NULL) {
	    seq = xmlXPathNodeSetCreate(NULL);
	    if (seq == NULL) {
		total = 0;
		goto error;
	    }
	}
	 
	pos = 0;
	cur = NULL;
	hasNsNodes = 0;
        do {
            cur = next(ctxt, cur);
            if (cur == NULL)
                break;

	     
            if ((first != NULL) && (*first != NULL)) {
		if (*first == cur)
		    break;
		if (((total % 256) == 0) &&
#ifdef XP_OPTIMIZED_NON_ELEM_COMPARISON
		    (xmlXPathCmpNodesExt(*first, cur) >= 0))
#else
		    (xmlXPathCmpNodes(*first, cur) >= 0))
#endif
		{
		    break;
		}
	    }
	    if ((last != NULL) && (*last != NULL)) {
		if (*last == cur)
		    break;
		if (((total % 256) == 0) &&
#ifdef XP_OPTIMIZED_NON_ELEM_COMPARISON
		    (xmlXPathCmpNodesExt(cur, *last) >= 0))
#else
		    (xmlXPathCmpNodes(cur, *last) >= 0))
#endif
		{
		    break;
		}
	    }

            total++;

#ifdef DEBUG_STEP
            xmlGenericError(xmlGenericErrorContext, " %s", cur->name);
#endif

	    switch (test) {
                case NODE_TEST_NONE:
		    total = 0;
                    STRANGE
		    goto error;
                case NODE_TEST_TYPE:
		     
		    if (type == NODE_TYPE_NODE) {
			switch (cur->type) {
			    case XML_DOCUMENT_NODE:
			    case XML_HTML_DOCUMENT_NODE:
#ifdef LIBXML_DOCB_ENABLED
			    case XML_DOCB_DOCUMENT_NODE:
#endif
			    case XML_ELEMENT_NODE:
			    case XML_ATTRIBUTE_NODE:
			    case XML_PI_NODE:
			    case XML_COMMENT_NODE:
			    case XML_CDATA_SECTION_NODE:
			    case XML_TEXT_NODE:
			    case XML_NAMESPACE_DECL:
				XP_TEST_HIT
				break;
			    default:
				break;
			}
		    } else if (cur->type == type) {
			if (type == XML_NAMESPACE_DECL)
			    XP_TEST_HIT_NS
			else
			    XP_TEST_HIT
		    } else if ((type == NODE_TYPE_TEXT) &&
			 (cur->type == XML_CDATA_SECTION_NODE))
		    {
			XP_TEST_HIT
		    }
		    break;
                case NODE_TEST_PI:
                    if ((cur->type == XML_PI_NODE) &&
                        ((name == NULL) || xmlStrEqual(name, cur->name)))
		    {
			XP_TEST_HIT
                    }
                    break;
                case NODE_TEST_ALL:
                    if (axis == AXIS_ATTRIBUTE) {
                        if (cur->type == XML_ATTRIBUTE_NODE)
			{
			    XP_TEST_HIT
                        }
                    } else if (axis == AXIS_NAMESPACE) {
                        if (cur->type == XML_NAMESPACE_DECL)
			{
			    XP_TEST_HIT_NS
                        }
                    } else {
                        if (cur->type == XML_ELEMENT_NODE) {
                            if (prefix == NULL)
			    {
				XP_TEST_HIT

                            } else if ((cur->ns != NULL) &&
				(xmlStrEqual(URI, cur->ns->href)))
			    {
				XP_TEST_HIT
                            }
                        }
                    }
                    break;
                case NODE_TEST_NS:{
                        TODO;
                        break;
                    }
                case NODE_TEST_NAME:
                    if (axis == AXIS_ATTRIBUTE) {
                        if (cur->type != XML_ATTRIBUTE_NODE)
			    break;
		    } else if (axis == AXIS_NAMESPACE) {
                        if (cur->type != XML_NAMESPACE_DECL)
			    break;
		    } else {
		        if (cur->type != XML_ELEMENT_NODE)
			    break;
		    }
                    switch (cur->type) {
                        case XML_ELEMENT_NODE:
                            if (xmlStrEqual(name, cur->name)) {
                                if (prefix == NULL) {
                                    if (cur->ns == NULL)
				    {
					XP_TEST_HIT
                                    }
                                } else {
                                    if ((cur->ns != NULL) &&
                                        (xmlStrEqual(URI, cur->ns->href)))
				    {
					XP_TEST_HIT
                                    }
                                }
                            }
                            break;
                        case XML_ATTRIBUTE_NODE:{
                                xmlAttrPtr attr = (xmlAttrPtr) cur;

                                if (xmlStrEqual(name, attr->name)) {
                                    if (prefix == NULL) {
                                        if ((attr->ns == NULL) ||
                                            (attr->ns->prefix == NULL))
					{
					    XP_TEST_HIT
                                        }
                                    } else {
                                        if ((attr->ns != NULL) &&
                                            (xmlStrEqual(URI,
					      attr->ns->href)))
					{
					    XP_TEST_HIT
                                        }
                                    }
                                }
                                break;
                            }
                        case XML_NAMESPACE_DECL:
                            if (cur->type == XML_NAMESPACE_DECL) {
                                xmlNsPtr ns = (xmlNsPtr) cur;

                                if ((ns->prefix != NULL) && (name != NULL)
                                    && (xmlStrEqual(ns->prefix, name)))
				{
				    XP_TEST_HIT_NS
                                }
                            }
                            break;
                        default:
                            break;
                    }
                    break;
	    }  
        } while (cur != NULL);

	goto apply_predicates;

axis_range_end:  
	 
	if (outSeq == NULL) {
	    outSeq = seq;
	    seq = NULL;
	} else
	    outSeq = mergeAndClear(outSeq, seq, 0);
	 
	if (toBool)
	    break;
	continue;

first_hit:  
	 
	if (outSeq == NULL) {
	    outSeq = seq;
	    seq = NULL;
	} else
	    outSeq = mergeAndClear(outSeq, seq, 0);
	break;

#ifdef DEBUG_STEP
	if (seq != NULL)
	    nbMatches += seq->nodeNr;
#endif

apply_predicates:  
         
        if ((predOp != NULL) && (seq->nodeNr > 0)) {
	     	    
	     	    
	     
	    size = seq->nodeNr;
	    if (hasPredicateRange != 0)
		newSize = xmlXPathCompOpEvalPositionalPredicate(ctxt,
		    predOp, seq, size, maxPos, maxPos, hasNsNodes);
	    else
		newSize = xmlXPathCompOpEvalPredicate(ctxt,
		    predOp, seq, size, hasNsNodes);

	    if (ctxt->error != XPATH_EXPRESSION_OK) {
		total = 0;
		goto error;
	    }
	     
	    if (newSize == 0) {
		 
		xmlXPathNodeSetClear(seq, hasNsNodes);
	    } else if (seq->nodeNr > 0) {
		 
		if (outSeq == NULL) {
		    if (size != newSize) {
			 
			outSeq = mergeAndClear(NULL, seq, 1);
		    } else {
			outSeq = seq;
			seq = NULL;
		    }
		} else
		    outSeq = mergeAndClear(outSeq, seq,
			(size != newSize) ? 1: 0);
		 
		if (toBool)
		    break;
	    }
        } else if (seq->nodeNr > 0) {
	     
	    if (outSeq == NULL) {
		outSeq = seq;
		seq = NULL;
	    } else {
		outSeq = mergeAndClear(outSeq, seq, 0);
	    }
	}
    }

error:
    if ((obj->boolval) && (obj->user != NULL)) {
	 
	ctxt->value->boolval = 1;
	ctxt->value->user = obj->user;
	obj->user = NULL;
	obj->boolval = 0;
    }
    xmlXPathReleaseObject(xpctxt, obj);

     
    if (outSeq == NULL) {
	if ((seq != NULL) && (seq->nodeNr == 0))
	    outSeq = seq;
	else
	    outSeq = xmlXPathNodeSetCreate(NULL);
         
    }
    if ((seq != NULL) && (seq != outSeq)) {
	 xmlXPathFreeNodeSet(seq);
    }
     
    valuePush(ctxt, xmlXPathCacheWrapNodeSet(xpctxt, outSeq));
     
    xpctxt->node = oldContextNode;

#ifdef DEBUG_STEP
    xmlGenericError(xmlGenericErrorContext,
	"\nExamined %d nodes, found %d nodes at that step\n",
	total, nbMatches);
#endif

    return(total);
}
