xmlXPathCompOpEval(xmlXPathParserContextPtr ctxt, xmlXPathStepOpPtr op)
{
    int total = 0;
    int equal, ret;
    xmlXPathCompExprPtr comp;
    xmlXPathObjectPtr arg1, arg2;
    xmlNodePtr bak;
    xmlDocPtr bakd;
    int pp;
    int cs;

    CHECK_ERROR0;
    comp = ctxt->comp;
    switch (op->op) {
        case XPATH_OP_END:
            return (0);
        case XPATH_OP_AND:
	    bakd = ctxt->context->doc;
	    bak = ctxt->context->node;
	    pp = ctxt->context->proximityPosition;
	    cs = ctxt->context->contextSize;
            total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch1]);
	    CHECK_ERROR0;
            xmlXPathBooleanFunction(ctxt, 1);
            if ((ctxt->value == NULL) || (ctxt->value->boolval == 0))
                return (total);
            arg2 = valuePop(ctxt);
	    ctxt->context->doc = bakd;
	    ctxt->context->node = bak;
	    ctxt->context->proximityPosition = pp;
	    ctxt->context->contextSize = cs;
            total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch2]);
	    if (ctxt->error) {
		xmlXPathFreeObject(arg2);
		return(0);
	    }
            xmlXPathBooleanFunction(ctxt, 1);
            arg1 = valuePop(ctxt);
            arg1->boolval &= arg2->boolval;
            valuePush(ctxt, arg1);
	    xmlXPathReleaseObject(ctxt->context, arg2);
            return (total);
        case XPATH_OP_OR:
	    bakd = ctxt->context->doc;
	    bak = ctxt->context->node;
	    pp = ctxt->context->proximityPosition;
	    cs = ctxt->context->contextSize;
            total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch1]);
	    CHECK_ERROR0;
            xmlXPathBooleanFunction(ctxt, 1);
            if ((ctxt->value == NULL) || (ctxt->value->boolval == 1))
                return (total);
            arg2 = valuePop(ctxt);
	    ctxt->context->doc = bakd;
	    ctxt->context->node = bak;
	    ctxt->context->proximityPosition = pp;
	    ctxt->context->contextSize = cs;
            total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch2]);
	    if (ctxt->error) {
		xmlXPathFreeObject(arg2);
		return(0);
	    }
            xmlXPathBooleanFunction(ctxt, 1);
            arg1 = valuePop(ctxt);
            arg1->boolval |= arg2->boolval;
            valuePush(ctxt, arg1);
	    xmlXPathReleaseObject(ctxt->context, arg2);
            return (total);
        case XPATH_OP_EQUAL:
	    bakd = ctxt->context->doc;
	    bak = ctxt->context->node;
	    pp = ctxt->context->proximityPosition;
	    cs = ctxt->context->contextSize;
            total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch1]);
	    CHECK_ERROR0;
	    ctxt->context->doc = bakd;
	    ctxt->context->node = bak;
	    ctxt->context->proximityPosition = pp;
	    ctxt->context->contextSize = cs;
            total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch2]);
	    CHECK_ERROR0;
	    if (op->value)
		equal = xmlXPathEqualValues(ctxt);
	    else
		equal = xmlXPathNotEqualValues(ctxt);
	    valuePush(ctxt, xmlXPathCacheNewBoolean(ctxt->context, equal));
            return (total);
        case XPATH_OP_CMP:
	    bakd = ctxt->context->doc;
	    bak = ctxt->context->node;
	    pp = ctxt->context->proximityPosition;
	    cs = ctxt->context->contextSize;
            total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch1]);
	    CHECK_ERROR0;
	    ctxt->context->doc = bakd;
	    ctxt->context->node = bak;
	    ctxt->context->proximityPosition = pp;
	    ctxt->context->contextSize = cs;
            total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch2]);
	    CHECK_ERROR0;
            ret = xmlXPathCompareValues(ctxt, op->value, op->value2);
	    valuePush(ctxt, xmlXPathCacheNewBoolean(ctxt->context, ret));
            return (total);
        case XPATH_OP_PLUS:
	    bakd = ctxt->context->doc;
	    bak = ctxt->context->node;
	    pp = ctxt->context->proximityPosition;
	    cs = ctxt->context->contextSize;
            total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch1]);
	    CHECK_ERROR0;
            if (op->ch2 != -1) {
		ctxt->context->doc = bakd;
		ctxt->context->node = bak;
		ctxt->context->proximityPosition = pp;
		ctxt->context->contextSize = cs;
                total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch2]);
	    }
	    CHECK_ERROR0;
            if (op->value == 0)
                xmlXPathSubValues(ctxt);
            else if (op->value == 1)
                xmlXPathAddValues(ctxt);
            else if (op->value == 2)
                xmlXPathValueFlipSign(ctxt);
            else if (op->value == 3) {
                CAST_TO_NUMBER;
                CHECK_TYPE0(XPATH_NUMBER);
            }
            return (total);
        case XPATH_OP_MULT:
	    bakd = ctxt->context->doc;
	    bak = ctxt->context->node;
	    pp = ctxt->context->proximityPosition;
	    cs = ctxt->context->contextSize;
            total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch1]);
	    CHECK_ERROR0;
	    ctxt->context->doc = bakd;
	    ctxt->context->node = bak;
	    ctxt->context->proximityPosition = pp;
	    ctxt->context->contextSize = cs;
            total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch2]);
	    CHECK_ERROR0;
            if (op->value == 0)
                xmlXPathMultValues(ctxt);
            else if (op->value == 1)
                xmlXPathDivValues(ctxt);
            else if (op->value == 2)
                xmlXPathModValues(ctxt);
            return (total);
        case XPATH_OP_UNION:
	    bakd = ctxt->context->doc;
	    bak = ctxt->context->node;
	    pp = ctxt->context->proximityPosition;
	    cs = ctxt->context->contextSize;
            total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch1]);
	    CHECK_ERROR0;
	    ctxt->context->doc = bakd;
	    ctxt->context->node = bak;
	    ctxt->context->proximityPosition = pp;
	    ctxt->context->contextSize = cs;
            total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch2]);
	    CHECK_ERROR0;
            CHECK_TYPE0(XPATH_NODESET);
            arg2 = valuePop(ctxt);

            CHECK_TYPE0(XPATH_NODESET);
            arg1 = valuePop(ctxt);

	    if ((arg1->nodesetval == NULL) ||
		((arg2->nodesetval != NULL) &&
		 (arg2->nodesetval->nodeNr != 0)))
	    {
		arg1->nodesetval = xmlXPathNodeSetMerge(arg1->nodesetval,
							arg2->nodesetval);
	    }

            valuePush(ctxt, arg1);
	    xmlXPathReleaseObject(ctxt->context, arg2);
            return (total);
        case XPATH_OP_ROOT:
            xmlXPathRoot(ctxt);
            return (total);
        case XPATH_OP_NODE:
            if (op->ch1 != -1)
                total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch1]);
	    CHECK_ERROR0;
            if (op->ch2 != -1)
                total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch2]);
	    CHECK_ERROR0;
	    valuePush(ctxt, xmlXPathCacheNewNodeSet(ctxt->context,
		ctxt->context->node));
            return (total);
        case XPATH_OP_RESET:
            if (op->ch1 != -1)
                total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch1]);
	    CHECK_ERROR0;
            if (op->ch2 != -1)
                total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch2]);
	    CHECK_ERROR0;
            ctxt->context->node = NULL;
            return (total);
        case XPATH_OP_COLLECT:{
                if (op->ch1 == -1)
                    return (total);

                total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch1]);
		CHECK_ERROR0;

                total += xmlXPathNodeCollectAndTest(ctxt, op, NULL, NULL, 0);
                return (total);
            }
        case XPATH_OP_VALUE:
            valuePush(ctxt,
                      xmlXPathCacheObjectCopy(ctxt->context,
			(xmlXPathObjectPtr) op->value4));
            return (total);
        case XPATH_OP_VARIABLE:{
		xmlXPathObjectPtr val;

                if (op->ch1 != -1)
                    total +=
                        xmlXPathCompOpEval(ctxt, &comp->steps[op->ch1]);
                if (op->value5 == NULL) {
		    val = xmlXPathVariableLookup(ctxt->context, op->value4);
		    if (val == NULL) {
			ctxt->error = XPATH_UNDEF_VARIABLE_ERROR;
			return(0);
		    }
                    valuePush(ctxt, val);
		} else {
                    const xmlChar *URI;

                    URI = xmlXPathNsLookup(ctxt->context, op->value5);
                    if (URI == NULL) {
                        xmlGenericError(xmlGenericErrorContext,
            "xmlXPathCompOpEval: variable %s bound to undefined prefix %s\n",
                                    (char *) op->value4, (char *)op->value5);
                        return (total);
                    }
		    val = xmlXPathVariableLookupNS(ctxt->context,
                                                       op->value4, URI);
		    if (val == NULL) {
			ctxt->error = XPATH_UNDEF_VARIABLE_ERROR;
			return(0);
		    }
                    valuePush(ctxt, val);
                }
                return (total);
            }
        case XPATH_OP_FUNCTION:{
                xmlXPathFunction func;
                const xmlChar *oldFunc, *oldFuncURI;
		int i;

                if (op->ch1 != -1)
                    total +=
                        xmlXPathCompOpEval(ctxt, &comp->steps[op->ch1]);
		if (ctxt->valueNr < op->value) {
		    xmlGenericError(xmlGenericErrorContext,
			    "xmlXPathCompOpEval: parameter error\n");
		    ctxt->error = XPATH_INVALID_OPERAND;
		    return (total);
		}
		for (i = 0; i < op->value; i++)
		    if (ctxt->valueTab[(ctxt->valueNr - 1) - i] == NULL) {
			xmlGenericError(xmlGenericErrorContext,
				"xmlXPathCompOpEval: parameter error\n");
			ctxt->error = XPATH_INVALID_OPERAND;
			return (total);
		    }
                if (op->cache != NULL)
                    XML_CAST_FPTR(func) = op->cache;
                else {
                    const xmlChar *URI = NULL;

                    if (op->value5 == NULL)
                        func =
                            xmlXPathFunctionLookup(ctxt->context,
                                                   op->value4);
                    else {
                        URI = xmlXPathNsLookup(ctxt->context, op->value5);
                        if (URI == NULL) {
                            xmlGenericError(xmlGenericErrorContext,
            "xmlXPathCompOpEval: function %s bound to undefined prefix %s\n",
                                    (char *)op->value4, (char *)op->value5);
                            return (total);
                        }
                        func = xmlXPathFunctionLookupNS(ctxt->context,
                                                        op->value4, URI);
                    }
                    if (func == NULL) {
                        xmlGenericError(xmlGenericErrorContext,
                                "xmlXPathCompOpEval: function %s not found\n",
                                        (char *)op->value4);
                        XP_ERROR0(XPATH_UNKNOWN_FUNC_ERROR);
                    }
                    op->cache = XML_CAST_FPTR(func);
                    op->cacheURI = (void *) URI;
                }
                oldFunc = ctxt->context->function;
                oldFuncURI = ctxt->context->functionURI;
                ctxt->context->function = op->value4;
                ctxt->context->functionURI = op->cacheURI;
                func(ctxt, op->value);
                ctxt->context->function = oldFunc;
                ctxt->context->functionURI = oldFuncURI;
                return (total);
            }
        case XPATH_OP_ARG:
	    bakd = ctxt->context->doc;
	    bak = ctxt->context->node;
	    pp = ctxt->context->proximityPosition;
	    cs = ctxt->context->contextSize;
            if (op->ch1 != -1)
                total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch1]);
	    ctxt->context->contextSize = cs;
	    ctxt->context->proximityPosition = pp;
	    ctxt->context->node = bak;
	    ctxt->context->doc = bakd;
	    CHECK_ERROR0;
            if (op->ch2 != -1) {
                total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch2]);
	        ctxt->context->doc = bakd;
	        ctxt->context->node = bak;
	        CHECK_ERROR0;
	    }
            return (total);
        case XPATH_OP_PREDICATE:
        case XPATH_OP_FILTER:{
                xmlXPathObjectPtr res;
                xmlXPathObjectPtr obj, tmp;
                xmlNodeSetPtr newset = NULL;
                xmlNodeSetPtr oldset;
                xmlNodePtr oldnode;
		xmlDocPtr oldDoc;
                int i;

                 
                if ((op->ch1 != -1) && (op->ch2 != -1) &&
#ifdef XP_OPTIMIZED_FILTER_FIRST
		     
                    ((comp->steps[op->ch1].op == XPATH_OP_SORT) ||  
		     (comp->steps[op->ch1].op == XPATH_OP_FILTER)) &&  
#else
		    (comp->steps[op->ch1].op == XPATH_OP_SORT) &&
#endif
                    (comp->steps[op->ch2].op == XPATH_OP_VALUE)) {  
                    xmlXPathObjectPtr val;

                    val = comp->steps[op->ch2].value4;
                    if ((val != NULL) && (val->type == XPATH_NUMBER) &&
                        (val->floatval == 1.0)) {
                        xmlNodePtr first = NULL;

                        total +=
                            xmlXPathCompOpEvalFirst(ctxt,
                                                    &comp->steps[op->ch1],
                                                    &first);
			CHECK_ERROR0;
                         
                        if ((ctxt->value != NULL) &&
                            (ctxt->value->type == XPATH_NODESET) &&
                            (ctxt->value->nodesetval != NULL) &&
                            (ctxt->value->nodesetval->nodeNr > 1))
                            ctxt->value->nodesetval->nodeNr = 1;
                        return (total);
                    }
                }
                 
                if ((op->ch1 != -1) && (op->ch2 != -1) &&
                    (comp->steps[op->ch1].op == XPATH_OP_SORT) &&
                    (comp->steps[op->ch2].op == XPATH_OP_SORT)) {
                    int f = comp->steps[op->ch2].ch1;

                    if ((f != -1) &&
                        (comp->steps[f].op == XPATH_OP_FUNCTION) &&
                        (comp->steps[f].value5 == NULL) &&
                        (comp->steps[f].value == 0) &&
                        (comp->steps[f].value4 != NULL) &&
                        (xmlStrEqual
                         (comp->steps[f].value4, BAD_CAST "last"))) {
                        xmlNodePtr last = NULL;

                        total +=
                            xmlXPathCompOpEvalLast(ctxt,
                                                   &comp->steps[op->ch1],
                                                   &last);
			CHECK_ERROR0;
                         
                        if ((ctxt->value != NULL) &&
                            (ctxt->value->type == XPATH_NODESET) &&
                            (ctxt->value->nodesetval != NULL) &&
                            (ctxt->value->nodesetval->nodeTab != NULL) &&
                            (ctxt->value->nodesetval->nodeNr > 1)) {
                            ctxt->value->nodesetval->nodeTab[0] =
                                ctxt->value->nodesetval->nodeTab[ctxt->
                                                                 value->
                                                                 nodesetval->
                                                                 nodeNr -
                                                                 1];
                            ctxt->value->nodesetval->nodeNr = 1;
                        }
                        return (total);
                    }
                }
		 
                if (op->ch1 != -1)
                    total +=
                        xmlXPathCompOpEval(ctxt, &comp->steps[op->ch1]);
		CHECK_ERROR0;
                if (op->ch2 == -1)
                    return (total);
                if (ctxt->value == NULL)
                    return (total);

                oldnode = ctxt->context->node;

#ifdef LIBXML_XPTR_ENABLED
                 
                if (ctxt->value->type == XPATH_LOCATIONSET) {
                    xmlLocationSetPtr newlocset = NULL;
                    xmlLocationSetPtr oldlocset;

                     
                    CHECK_TYPE0(XPATH_LOCATIONSET);
                    obj = valuePop(ctxt);
                    oldlocset = obj->user;
                    ctxt->context->node = NULL;

                    if ((oldlocset == NULL) || (oldlocset->locNr == 0)) {
                        ctxt->context->contextSize = 0;
                        ctxt->context->proximityPosition = 0;
                        if (op->ch2 != -1)
                            total +=
                                xmlXPathCompOpEval(ctxt,
                                                   &comp->steps[op->ch2]);
                        res = valuePop(ctxt);
                        if (res != NULL) {
			    xmlXPathReleaseObject(ctxt->context, res);
			}
                        valuePush(ctxt, obj);
                        CHECK_ERROR0;
                        return (total);
                    }
                    newlocset = xmlXPtrLocationSetCreate(NULL);

                    for (i = 0; i < oldlocset->locNr; i++) {
                         
                        ctxt->context->node = oldlocset->locTab[i]->user;
                        ctxt->context->contextSize = oldlocset->locNr;
                        ctxt->context->proximityPosition = i + 1;
			tmp = xmlXPathCacheNewNodeSet(ctxt->context,
			    ctxt->context->node);
                        valuePush(ctxt, tmp);

                        if (op->ch2 != -1)
                            total +=
                                xmlXPathCompOpEval(ctxt,
                                                   &comp->steps[op->ch2]);
			if (ctxt->error != XPATH_EXPRESSION_OK) {
			    xmlXPathFreeObject(obj);
			    return(0);
			}

                         
                        res = valuePop(ctxt);
                        if (xmlXPathEvaluatePredicateResult(ctxt, res)) {
                            xmlXPtrLocationSetAdd(newlocset,
                                                  xmlXPathObjectCopy
                                                  (oldlocset->locTab[i]));
                        }

                         
                        if (res != NULL) {
			    xmlXPathReleaseObject(ctxt->context, res);
			}
                        if (ctxt->value == tmp) {
                            res = valuePop(ctxt);
			    xmlXPathReleaseObject(ctxt->context, res);
                        }

                        ctxt->context->node = NULL;
                    }

                     
		    xmlXPathReleaseObject(ctxt->context, obj);
                    ctxt->context->node = NULL;
                    ctxt->context->contextSize = -1;
                    ctxt->context->proximityPosition = -1;
                    valuePush(ctxt, xmlXPtrWrapLocationSet(newlocset));
                    ctxt->context->node = oldnode;
                    return (total);
                }
#endif  

                 
                CHECK_TYPE0(XPATH_NODESET);
                obj = valuePop(ctxt);
                oldset = obj->nodesetval;

                oldnode = ctxt->context->node;
		oldDoc = ctxt->context->doc;
                ctxt->context->node = NULL;

                if ((oldset == NULL) || (oldset->nodeNr == 0)) {
                    ctxt->context->contextSize = 0;
                    ctxt->context->proximityPosition = 0;
 
                    valuePush(ctxt, obj);
                    ctxt->context->node = oldnode;
                    CHECK_ERROR0;
                } else {
		    tmp = NULL;
                     
                    newset = xmlXPathNodeSetCreate(NULL);
		     
                    for (i = 0; i < oldset->nodeNr; i++) {
                         
                        ctxt->context->node = oldset->nodeTab[i];
			if ((oldset->nodeTab[i]->type != XML_NAMESPACE_DECL) &&
			    (oldset->nodeTab[i]->doc != NULL))
		            ctxt->context->doc = oldset->nodeTab[i]->doc;
			if (tmp == NULL) {
			    tmp = xmlXPathCacheNewNodeSet(ctxt->context,
				ctxt->context->node);
			} else {
			    xmlXPathNodeSetAddUnique(tmp->nodesetval,
				ctxt->context->node);
			}
                        valuePush(ctxt, tmp);
                        ctxt->context->contextSize = oldset->nodeNr;
                        ctxt->context->proximityPosition = i + 1;
			 
                        if (op->ch2 != -1)
                            total +=
                                xmlXPathCompOpEval(ctxt,
                                                   &comp->steps[op->ch2]);
			if (ctxt->error != XPATH_EXPRESSION_OK) {
			    xmlXPathFreeNodeSet(newset);
			    xmlXPathFreeObject(obj);
			    return(0);
			}

                         
			 
                        res = valuePop(ctxt);
                        if (xmlXPathEvaluatePredicateResult(ctxt, res)) {
                            xmlXPathNodeSetAdd(newset, oldset->nodeTab[i]);
                        }

                         
                        if (res != NULL) {
			    xmlXPathReleaseObject(ctxt->context, res);
			}
                        if (ctxt->value == tmp) {
                            valuePop(ctxt);
			    xmlXPathNodeSetClear(tmp->nodesetval, 1);
			     
                        } else
			    tmp = NULL;
                        ctxt->context->node = NULL;
                    }
		    if (tmp != NULL)
			xmlXPathReleaseObject(ctxt->context, tmp);
                     
		    xmlXPathReleaseObject(ctxt->context, obj);
                    ctxt->context->node = NULL;
                    ctxt->context->contextSize = -1;
                    ctxt->context->proximityPosition = -1;
		     
		    ctxt->context->doc = oldDoc;
		    valuePush(ctxt,
			xmlXPathCacheWrapNodeSet(ctxt->context, newset));
                }
                ctxt->context->node = oldnode;
                return (total);
            }
        case XPATH_OP_SORT:
            if (op->ch1 != -1)
                total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch1]);
	    CHECK_ERROR0;
            if ((ctxt->value != NULL) &&
                (ctxt->value->type == XPATH_NODESET) &&
                (ctxt->value->nodesetval != NULL) &&
		(ctxt->value->nodesetval->nodeNr > 1))
	    {
                xmlXPathNodeSetSort(ctxt->value->nodesetval);
	    }
            return (total);
#ifdef LIBXML_XPTR_ENABLED
        case XPATH_OP_RANGETO:{
                xmlXPathObjectPtr range;
                xmlXPathObjectPtr res, obj;
                xmlXPathObjectPtr tmp;
                xmlLocationSetPtr newlocset = NULL;
		    xmlLocationSetPtr oldlocset;
                xmlNodeSetPtr oldset;
                int i, j;

                if (op->ch1 != -1)
                    total +=
                        xmlXPathCompOpEval(ctxt, &comp->steps[op->ch1]);
                if (op->ch2 == -1)
                    return (total);

                if (ctxt->value->type == XPATH_LOCATIONSET) {
                     
                    CHECK_TYPE0(XPATH_LOCATIONSET);
                    obj = valuePop(ctxt);
                    oldlocset = obj->user;

                    if ((oldlocset == NULL) || (oldlocset->locNr == 0)) {
		        ctxt->context->node = NULL;
                        ctxt->context->contextSize = 0;
                        ctxt->context->proximityPosition = 0;
                        total += xmlXPathCompOpEval(ctxt,&comp->steps[op->ch2]);
                        res = valuePop(ctxt);
                        if (res != NULL) {
			    xmlXPathReleaseObject(ctxt->context, res);
			}
                        valuePush(ctxt, obj);
                        CHECK_ERROR0;
                        return (total);
                    }
                    newlocset = xmlXPtrLocationSetCreate(NULL);

                    for (i = 0; i < oldlocset->locNr; i++) {
                         
                        ctxt->context->node = oldlocset->locTab[i]->user;
                        ctxt->context->contextSize = oldlocset->locNr;
                        ctxt->context->proximityPosition = i + 1;
			tmp = xmlXPathCacheNewNodeSet(ctxt->context,
			    ctxt->context->node);
                        valuePush(ctxt, tmp);

                        if (op->ch2 != -1)
                            total +=
                                xmlXPathCompOpEval(ctxt,
                                                   &comp->steps[op->ch2]);
			if (ctxt->error != XPATH_EXPRESSION_OK) {
			    xmlXPathFreeObject(obj);
			    return(0);
			}

                        res = valuePop(ctxt);
			if (res->type == XPATH_LOCATIONSET) {
			    xmlLocationSetPtr rloc =
			        (xmlLocationSetPtr)res->user;
			    for (j=0; j<rloc->locNr; j++) {
			        range = xmlXPtrNewRange(
				  oldlocset->locTab[i]->user,
				  oldlocset->locTab[i]->index,
				  rloc->locTab[j]->user2,
				  rloc->locTab[j]->index2);
				if (range != NULL) {
				    xmlXPtrLocationSetAdd(newlocset, range);
				}
			    }
			} else {
			    range = xmlXPtrNewRangeNodeObject(
				(xmlNodePtr)oldlocset->locTab[i]->user, res);
                            if (range != NULL) {
                                xmlXPtrLocationSetAdd(newlocset,range);
			    }
                        }

                         
                        if (res != NULL) {
			    xmlXPathReleaseObject(ctxt->context, res);
			}
                        if (ctxt->value == tmp) {
                            res = valuePop(ctxt);
			    xmlXPathReleaseObject(ctxt->context, res);
                        }

                        ctxt->context->node = NULL;
                    }
		} else {	 
                    CHECK_TYPE0(XPATH_NODESET);
                    obj = valuePop(ctxt);
                    oldset = obj->nodesetval;
                    ctxt->context->node = NULL;

                    newlocset = xmlXPtrLocationSetCreate(NULL);

                    if (oldset != NULL) {
                        for (i = 0; i < oldset->nodeNr; i++) {
                             
                            ctxt->context->node = oldset->nodeTab[i];
			     
			    tmp = xmlXPathCacheNewNodeSet(ctxt->context,
				ctxt->context->node);
                            valuePush(ctxt, tmp);

                            if (op->ch2 != -1)
                                total +=
                                    xmlXPathCompOpEval(ctxt,
                                                   &comp->steps[op->ch2]);
			    if (ctxt->error != XPATH_EXPRESSION_OK) {
				xmlXPathFreeObject(obj);
				return(0);
			    }

                            res = valuePop(ctxt);
                            range =
                                xmlXPtrNewRangeNodeObject(oldset->nodeTab[i],
                                                      res);
                            if (range != NULL) {
                                xmlXPtrLocationSetAdd(newlocset, range);
                            }

                             
                            if (res != NULL) {
				xmlXPathReleaseObject(ctxt->context, res);
			    }
                            if (ctxt->value == tmp) {
                                res = valuePop(ctxt);
				xmlXPathReleaseObject(ctxt->context, res);
                            }

                            ctxt->context->node = NULL;
                        }
                    }
                }

                 
		xmlXPathReleaseObject(ctxt->context, obj);
                ctxt->context->node = NULL;
                ctxt->context->contextSize = -1;
                ctxt->context->proximityPosition = -1;
                valuePush(ctxt, xmlXPtrWrapLocationSet(newlocset));
                return (total);
            }
#endif  
    }
