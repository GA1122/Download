FilterEncodingNode* FLTSimplify(FilterEncodingNode *psFilterNode,
                                int* pnEvaluation)
{
    *pnEvaluation = -1;

     
     
    if( psFilterNode->eType ==  FILTER_NODE_TYPE_COMPARISON &&
        (strcmp(psFilterNode->pszValue, "PropertyIsNull") == 0 ||
         strcmp(psFilterNode->pszValue, "PropertyIsNil") == 0 ) &&
        psFilterNode->psLeftNode != NULL &&
        psFilterNode->psLeftNode->eType == FILTER_NODE_TYPE_PROPERTYNAME )
    {
        if( strcmp(psFilterNode->pszValue, "PropertyIsNull") == 0 &&
            FLTIsGMLDefaultProperty(psFilterNode->psLeftNode->pszValue) &&
            strcmp(psFilterNode->psLeftNode->pszValue, "@gml:id") != 0  &&
            strcmp(psFilterNode->psLeftNode->pszValue, "gml:boundedBy") != 0)
            *pnEvaluation = MS_TRUE;
        else
            *pnEvaluation = MS_FALSE;
        FLTFreeFilterEncodingNode(psFilterNode);
        return NULL;
    }

    if( psFilterNode->eType ==  FILTER_NODE_TYPE_LOGICAL &&
        strcasecmp(psFilterNode->pszValue, "NOT") == 0 &&
        psFilterNode->psLeftNode != NULL )
    {
        int nEvaluation;
        psFilterNode->psLeftNode = FLTSimplify(psFilterNode->psLeftNode,
                                               &nEvaluation);
        if( psFilterNode->psLeftNode == NULL )
        {
            *pnEvaluation = 1 - nEvaluation;
            FLTFreeFilterEncodingNode(psFilterNode);
            return NULL;
        }
    }

    if( psFilterNode->eType ==  FILTER_NODE_TYPE_LOGICAL &&
        (strcasecmp(psFilterNode->pszValue, "AND") == 0 ||
         strcasecmp(psFilterNode->pszValue, "OR") == 0) &&
        psFilterNode->psLeftNode != NULL &&
        psFilterNode->psRightNode != NULL )
    {
        FilterEncodingNode* psOtherNode;
        int nEvaluation;
        int nExpectedValForFastExit;
        psFilterNode->psLeftNode = FLTSimplify(psFilterNode->psLeftNode,
                                               &nEvaluation);

        if( strcasecmp(psFilterNode->pszValue, "AND") == 0 )
            nExpectedValForFastExit = MS_FALSE;
        else
            nExpectedValForFastExit = MS_TRUE;

        if( psFilterNode->psLeftNode == NULL )
        {
            if( nEvaluation == nExpectedValForFastExit )
            {
                *pnEvaluation = nEvaluation;
                FLTFreeFilterEncodingNode(psFilterNode);
                return NULL;
            }
            psOtherNode = psFilterNode->psRightNode;
            psFilterNode->psRightNode = NULL;
            FLTFreeFilterEncodingNode(psFilterNode);
            return FLTSimplify(psOtherNode, pnEvaluation);
        }
 
        psFilterNode->psRightNode = FLTSimplify(psFilterNode->psRightNode,
                                                &nEvaluation);
        if( psFilterNode->psRightNode == NULL )
        {
            if( nEvaluation == nExpectedValForFastExit )
            {
                *pnEvaluation = nEvaluation;
                FLTFreeFilterEncodingNode(psFilterNode);
                return NULL;
            }
            psOtherNode = psFilterNode->psLeftNode;
            psFilterNode->psLeftNode = NULL;
            FLTFreeFilterEncodingNode(psFilterNode);
            return FLTSimplify(psOtherNode, pnEvaluation);
        }
    }

    return psFilterNode;
}