void FLTFreeFilterEncodingNode(FilterEncodingNode *psFilterNode)
{
  if (psFilterNode) {
    if (psFilterNode->psLeftNode) {
      FLTFreeFilterEncodingNode(psFilterNode->psLeftNode);
      psFilterNode->psLeftNode = NULL;
    }
    if (psFilterNode->psRightNode) {
      FLTFreeFilterEncodingNode(psFilterNode->psRightNode);
      psFilterNode->psRightNode = NULL;
    }

    if (psFilterNode->pszSRS)
      free( psFilterNode->pszSRS);

    if( psFilterNode->pOther ) {
      if (psFilterNode->pszValue != NULL &&
          strcasecmp(psFilterNode->pszValue, "PropertyIsLike") == 0) {
        FEPropertyIsLike* propIsLike = (FEPropertyIsLike *)psFilterNode->pOther;
        if( propIsLike->pszWildCard )
          free( propIsLike->pszWildCard );
        if( propIsLike->pszSingleChar )
          free( propIsLike->pszSingleChar );
        if( propIsLike->pszEscapeChar )
          free( propIsLike->pszEscapeChar );
      } else if (FLTIsGeometryFilterNodeType(psFilterNode->eType)) {
        msFreeShape((shapeObj *)(psFilterNode->pOther));
      }
       
       
      free( psFilterNode->pOther );
    }

     
    if( psFilterNode->pszValue )
      free( psFilterNode->pszValue );

    free(psFilterNode);
  }
}
