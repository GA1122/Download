void FLTInsertElementInNode(FilterEncodingNode *psFilterNode,
                            CPLXMLNode *psXMLNode)
{
  int nStrLength = 0;
  char *pszTmp = NULL;
  FilterEncodingNode *psCurFilNode= NULL;
  CPLXMLNode *psCurXMLNode = NULL;
  CPLXMLNode *psTmpNode = NULL;
  CPLXMLNode *psFeatureIdNode = NULL;
  const char *pszFeatureId=NULL;
  char *pszFeatureIdList=NULL;

  if (psFilterNode && psXMLNode && psXMLNode->pszValue) {
    psFilterNode->pszValue = msStrdup(psXMLNode->pszValue);
    psFilterNode->psLeftNode = NULL;
    psFilterNode->psRightNode = NULL;

     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
    if (FLTIsLogicalFilterType(psXMLNode->pszValue)) {
      psFilterNode->eType = FILTER_NODE_TYPE_LOGICAL;
      if (strcasecmp(psFilterNode->pszValue, "AND") == 0 ||
          strcasecmp(psFilterNode->pszValue, "OR") == 0) {
        CPLXMLNode* psFirstNode = FLTGetFirstChildNode(psXMLNode);
        CPLXMLNode* psSecondNode = FLTGetNextSibblingNode(psFirstNode);
        if (psFirstNode && psSecondNode) {
           
          CPLXMLNode* psNextNode = FLTGetNextSibblingNode(psSecondNode);
          if (psNextNode == NULL) {
            psFilterNode->psLeftNode = FLTCreateFilterEncodingNode();
            FLTInsertElementInNode(psFilterNode->psLeftNode, psFirstNode);
            psFilterNode->psRightNode = FLTCreateFilterEncodingNode();
            FLTInsertElementInNode(psFilterNode->psRightNode, psSecondNode);
          } else {
            psCurXMLNode = psFirstNode;
            psCurFilNode = psFilterNode;
            while(psCurXMLNode) {
              psNextNode = FLTGetNextSibblingNode(psCurXMLNode);
              if (FLTGetNextSibblingNode(psNextNode)) {
                psCurFilNode->psLeftNode = FLTCreateFilterEncodingNode();
                FLTInsertElementInNode(psCurFilNode->psLeftNode, psCurXMLNode);
                psCurFilNode->psRightNode = FLTCreateFilterEncodingNode();
                psCurFilNode->psRightNode->eType = FILTER_NODE_TYPE_LOGICAL;
                psCurFilNode->psRightNode->pszValue = msStrdup(psFilterNode->pszValue);

                psCurFilNode = psCurFilNode->psRightNode;
                psCurXMLNode = psNextNode;
              } else {  
                psCurFilNode->psLeftNode = FLTCreateFilterEncodingNode();
                FLTInsertElementInNode(psCurFilNode->psLeftNode, psCurXMLNode);

                psCurFilNode->psRightNode = FLTCreateFilterEncodingNode();
                FLTInsertElementInNode(psCurFilNode->psRightNode, psNextNode);
                break;
              }
            }
          }
        }
        else
          psFilterNode->eType = FILTER_NODE_TYPE_UNDEFINED;
      } else if (strcasecmp(psFilterNode->pszValue, "NOT") == 0) {
        CPLXMLNode* psFirstNode = FLTGetFirstChildNode(psXMLNode);
        if (psFirstNode) {
          psFilterNode->psLeftNode = FLTCreateFilterEncodingNode();
          FLTInsertElementInNode(psFilterNode->psLeftNode,
                                 psFirstNode);
        }
        else
          psFilterNode->eType = FILTER_NODE_TYPE_UNDEFINED;
      } else
        psFilterNode->eType = FILTER_NODE_TYPE_UNDEFINED;
    } 
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
    else if (FLTIsSpatialFilterType(psXMLNode->pszValue)) {
      psFilterNode->eType = FILTER_NODE_TYPE_SPATIAL;

      if (strcasecmp(psXMLNode->pszValue, "BBOX") == 0) {
        char *pszSRS = NULL;
        const char* pszPropertyName = NULL;
        CPLXMLNode *psBox = NULL, *psEnvelope=NULL;
        rectObj sBox;

        int bCoordinatesValid = 0;

        pszPropertyName = FLTGetPropertyName(psXMLNode);
        psBox = CPLGetXMLNode(psXMLNode, "Box");
        if (!psBox)
          psBox = CPLGetXMLNode(psXMLNode, "BoxType");

         
        if (psBox)
          bCoordinatesValid = FLTParseGMLBox(psBox, &sBox, &pszSRS);
        else if ((psEnvelope = CPLGetXMLNode(psXMLNode, "Envelope")))
          bCoordinatesValid = FLTParseGMLEnvelope(psEnvelope, &sBox, &pszSRS);

        if (bCoordinatesValid) {
           
          if (pszSRS)
            psFilterNode->pszSRS = pszSRS;

          psFilterNode->psLeftNode = FLTCreateFilterEncodingNode();
          psFilterNode->psLeftNode->eType =  FILTER_NODE_TYPE_PROPERTYNAME;
           
           
           
           
          if( pszPropertyName != NULL ) {
            psFilterNode->psLeftNode->pszValue = msStrdup(pszPropertyName);
          }

           
          psFilterNode->psRightNode = FLTCreateFilterEncodingNode();
          psFilterNode->psRightNode->eType = FILTER_NODE_TYPE_BBOX;
          psFilterNode->psRightNode->pOther =
          (rectObj *)msSmallMalloc(sizeof(rectObj));
          ((rectObj *)psFilterNode->psRightNode->pOther)->minx = sBox.minx;
          ((rectObj *)psFilterNode->psRightNode->pOther)->miny = sBox.miny;
          ((rectObj *)psFilterNode->psRightNode->pOther)->maxx = sBox.maxx;
          ((rectObj *)psFilterNode->psRightNode->pOther)->maxy =  sBox.maxy;
        } else {
          msFree(pszSRS);
          psFilterNode->eType = FILTER_NODE_TYPE_UNDEFINED;
        }
      } else if (strcasecmp(psXMLNode->pszValue, "DWithin") == 0 ||
                 strcasecmp(psXMLNode->pszValue, "Beyond") == 0)

      {
        shapeObj *psShape = NULL;
        int bPoint = 0, bLine = 0, bPolygon = 0;
        const char *pszUnits = NULL;
        const char* pszDistance = NULL;
        const char* pszPropertyName;
        char *pszSRS = NULL;

        CPLXMLNode *psGMLElement = NULL, *psDistance=NULL;

        pszPropertyName = FLTGetPropertyName(psXMLNode);

        psGMLElement = FLTFindGeometryNode(psXMLNode, &bPoint, &bLine, &bPolygon);

        psDistance = CPLGetXMLNode(psXMLNode, "Distance");
        if( psDistance != NULL )
            pszDistance = CPLGetXMLValue(psDistance, NULL, NULL );
        if (pszPropertyName != NULL && psGMLElement && psDistance != NULL ) {
          pszUnits = CPLGetXMLValue(psDistance, "units", NULL);
          if( pszUnits == NULL )  
              pszUnits = CPLGetXMLValue(psDistance, "uom", NULL);
          psShape = (shapeObj *)msSmallMalloc(sizeof(shapeObj));
          msInitShape(psShape);
          if (FLTShapeFromGMLTree(psGMLElement, psShape, &pszSRS))
          {
             
            if (pszSRS)
              psFilterNode->pszSRS = pszSRS;

            psFilterNode->psLeftNode = FLTCreateFilterEncodingNode();
            psFilterNode->psLeftNode->eType = FILTER_NODE_TYPE_PROPERTYNAME;
            psFilterNode->psLeftNode->pszValue = msStrdup(pszPropertyName);

            psFilterNode->psRightNode = FLTCreateFilterEncodingNode();
            if (bPoint)
              psFilterNode->psRightNode->eType = FILTER_NODE_TYPE_GEOMETRY_POINT;
            else if (bLine)
              psFilterNode->psRightNode->eType = FILTER_NODE_TYPE_GEOMETRY_LINE;
            else if (bPolygon)
              psFilterNode->psRightNode->eType = FILTER_NODE_TYPE_GEOMETRY_POLYGON;
            psFilterNode->psRightNode->pOther = (shapeObj *)psShape;
             
            psFilterNode->psRightNode->pszValue = msStrdup(pszDistance);
            if (pszUnits) {
              psFilterNode->psRightNode->pszValue= msStringConcatenate(psFilterNode->psRightNode->pszValue, ";");
              psFilterNode->psRightNode->pszValue= msStringConcatenate(psFilterNode->psRightNode->pszValue, pszUnits);
            }
          }
          else
          {
              free(psShape);
              msFree(pszSRS);
              psFilterNode->eType = FILTER_NODE_TYPE_UNDEFINED;
          }
        } else
          psFilterNode->eType = FILTER_NODE_TYPE_UNDEFINED;
      } else if (strcasecmp(psXMLNode->pszValue, "Intersect") == 0 ||
                 strcasecmp(psXMLNode->pszValue, "Intersects") == 0 ||
                 strcasecmp(psXMLNode->pszValue, "Equals") == 0 ||
                 strcasecmp(psXMLNode->pszValue, "Disjoint") == 0 ||
                 strcasecmp(psXMLNode->pszValue, "Touches") == 0 ||
                 strcasecmp(psXMLNode->pszValue, "Crosses") == 0 ||
                 strcasecmp(psXMLNode->pszValue, "Within") == 0 ||
                 strcasecmp(psXMLNode->pszValue, "Contains") == 0 ||
                 strcasecmp(psXMLNode->pszValue, "Overlaps") == 0) {
        shapeObj *psShape = NULL;
        int  bLine = 0, bPolygon = 0, bPoint=0;
        char *pszSRS = NULL;
        const char* pszPropertyName;

        CPLXMLNode *psGMLElement = NULL;

        pszPropertyName = FLTGetPropertyName(psXMLNode);

        psGMLElement = FLTFindGeometryNode(psXMLNode, &bPoint, &bLine, &bPolygon);

        if (pszPropertyName != NULL && psGMLElement) {
          psShape = (shapeObj *)msSmallMalloc(sizeof(shapeObj));
          msInitShape(psShape);
          if (FLTShapeFromGMLTree(psGMLElement, psShape, &pszSRS))
          {
             
            if (pszSRS)
              psFilterNode->pszSRS = pszSRS;

            psFilterNode->psLeftNode = FLTCreateFilterEncodingNode();
            psFilterNode->psLeftNode->eType = FILTER_NODE_TYPE_PROPERTYNAME;
            psFilterNode->psLeftNode->pszValue = msStrdup(pszPropertyName);

            psFilterNode->psRightNode = FLTCreateFilterEncodingNode();
            if (bPoint)
              psFilterNode->psRightNode->eType = FILTER_NODE_TYPE_GEOMETRY_POINT;
            else if (bLine)
              psFilterNode->psRightNode->eType = FILTER_NODE_TYPE_GEOMETRY_LINE;
            else if (bPolygon)
              psFilterNode->psRightNode->eType = FILTER_NODE_TYPE_GEOMETRY_POLYGON;
            psFilterNode->psRightNode->pOther = (shapeObj *)psShape;

          }
          else
          {
              free(psShape);
              msFree(pszSRS);
              psFilterNode->eType = FILTER_NODE_TYPE_UNDEFINED;
          }
        } else
          psFilterNode->eType = FILTER_NODE_TYPE_UNDEFINED;
      }


    } 


     
     
     
    else if (FLTIsComparisonFilterType(psXMLNode->pszValue)) {
      psFilterNode->eType = FILTER_NODE_TYPE_COMPARISON;
       
       
       
       
       
       
       
       
       
       
      if (FLTIsBinaryComparisonFilterType(psXMLNode->pszValue)) {
        const char* pszPropertyName = FLTGetPropertyName(psXMLNode);
        if (pszPropertyName != NULL ) {

          psFilterNode->psLeftNode = FLTCreateFilterEncodingNode();
          psFilterNode->psLeftNode->eType = FILTER_NODE_TYPE_PROPERTYNAME;
          psFilterNode->psLeftNode->pszValue = msStrdup(pszPropertyName);

          psTmpNode = CPLSearchXMLNode(psXMLNode,  "Literal");
          if (psTmpNode) {
            const char* pszLiteral = CPLGetXMLValue(psTmpNode, NULL, NULL);

            psFilterNode->psRightNode = FLTCreateBinaryCompFilterEncodingNode();
            psFilterNode->psRightNode->eType = FILTER_NODE_TYPE_LITERAL;

            if (pszLiteral != NULL) {
              const char* pszMatchCase;

              psFilterNode->psRightNode->pszValue = msStrdup(pszLiteral);
              
              pszMatchCase = CPLGetXMLValue(psXMLNode, "matchCase", NULL);

               
              if( pszMatchCase != NULL && strcasecmp( pszMatchCase, "false") == 0) {
                (*(int *)psFilterNode->psRightNode->pOther) = 1;
              }

            }
             
             
             
            else
              psFilterNode->psRightNode->pszValue = NULL;
          }
        }
        if (psFilterNode->psLeftNode == NULL || psFilterNode->psRightNode == NULL)
          psFilterNode->eType = FILTER_NODE_TYPE_UNDEFINED;
      }

       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
      else if (strcasecmp(psXMLNode->pszValue, "PropertyIsBetween") == 0) {
        const char* pszPropertyName = FLTGetPropertyName(psXMLNode);
        CPLXMLNode* psLowerBoundary = CPLGetXMLNode(psXMLNode, "LowerBoundary");
        CPLXMLNode* psUpperBoundary = CPLGetXMLNode(psXMLNode, "UpperBoundary");
        const char* pszLowerNode = NULL;
        const char* pszUpperNode = NULL;
        if( psLowerBoundary != NULL )
        {
           
          if (CPLGetXMLNode(psLowerBoundary, "Literal") != NULL)
            pszLowerNode = CPLGetXMLValue(psLowerBoundary, "Literal", NULL);
          else
            pszLowerNode = CPLGetXMLValue(psLowerBoundary, NULL, NULL);
        }
        if( psUpperBoundary != NULL )
        {
           if (CPLGetXMLNode(psUpperBoundary, "Literal") != NULL)
            pszUpperNode = CPLGetXMLValue(psUpperBoundary, "Literal", NULL);
          else
            pszUpperNode = CPLGetXMLValue(psUpperBoundary, NULL, NULL);
        }
        if (pszPropertyName != NULL && pszLowerNode != NULL && pszUpperNode != NULL) {
          psFilterNode->psLeftNode = FLTCreateFilterEncodingNode();

          psFilterNode->psLeftNode->eType = FILTER_NODE_TYPE_PROPERTYNAME;
          psFilterNode->psLeftNode->pszValue = msStrdup(pszPropertyName);

          psFilterNode->psRightNode = FLTCreateFilterEncodingNode();
          psFilterNode->psRightNode->eType = FILTER_NODE_TYPE_BOUNDARY;

           
          nStrLength = strlen(pszLowerNode) + strlen(pszUpperNode) + 2;

          psFilterNode->psRightNode->pszValue =
              (char *)malloc(sizeof(char)*(nStrLength));
          strcpy( psFilterNode->psRightNode->pszValue, pszLowerNode);
          strlcat(psFilterNode->psRightNode->pszValue, ";", nStrLength);
          strlcat(psFilterNode->psRightNode->pszValue, pszUpperNode, nStrLength);


        } else
          psFilterNode->eType = FILTER_NODE_TYPE_UNDEFINED;

      } 
       
       
       
       
       
       
       
       
       
       
      else if (strcasecmp(psXMLNode->pszValue, "PropertyIsLike") == 0) {
        const char* pszPropertyName = FLTGetPropertyName(psXMLNode);
        const char* pszLiteral = CPLGetXMLValue(psXMLNode, "Literal", NULL);
        const char* pszWildCard = CPLGetXMLValue(psXMLNode, "wildCard", NULL);
        const char* pszSingleChar = CPLGetXMLValue(psXMLNode, "singleChar", NULL);
        const char* pszEscapeChar = CPLGetXMLValue(psXMLNode, "escape", NULL);
        if( pszEscapeChar == NULL )
            pszEscapeChar = CPLGetXMLValue(psXMLNode, "escapeChar", NULL);
        if (pszPropertyName != NULL && pszLiteral != NULL &&
            pszWildCard != NULL && pszSingleChar != NULL && pszEscapeChar != NULL)
        {
          FEPropertyIsLike* propIsLike;

          propIsLike = (FEPropertyIsLike *)malloc(sizeof(FEPropertyIsLike));

          psFilterNode->pOther = propIsLike;
          propIsLike->bCaseInsensitive = 0;
          propIsLike->pszWildCard = msStrdup(pszWildCard);
          propIsLike->pszSingleChar = msStrdup(pszSingleChar);
          propIsLike->pszEscapeChar = msStrdup(pszEscapeChar);

          pszTmp = (char *)CPLGetXMLValue(psXMLNode, "matchCase", NULL);
          if (pszTmp && strcasecmp(pszTmp, "false") == 0) {
            propIsLike->bCaseInsensitive =1;
          }
           
           
           
          psFilterNode->psLeftNode = FLTCreateFilterEncodingNode();

          psFilterNode->psLeftNode->pszValue = msStrdup(pszPropertyName);
          psFilterNode->psLeftNode->eType = FILTER_NODE_TYPE_PROPERTYNAME;

          psFilterNode->psRightNode = FLTCreateFilterEncodingNode();

          psFilterNode->psRightNode->pszValue = msStrdup(pszLiteral);

          psFilterNode->psRightNode->eType = FILTER_NODE_TYPE_LITERAL;
        } else
          psFilterNode->eType = FILTER_NODE_TYPE_UNDEFINED;

      }

      else if (strcasecmp(psXMLNode->pszValue, "PropertyIsNull") == 0) {
        const char* pszPropertyName = FLTGetPropertyName(psXMLNode);
        if( pszPropertyName != NULL )
        {
            psFilterNode->psLeftNode = FLTCreateFilterEncodingNode();
            psFilterNode->psLeftNode->pszValue = msStrdup(pszPropertyName);
            psFilterNode->psLeftNode->eType = FILTER_NODE_TYPE_PROPERTYNAME;
        }  else
          psFilterNode->eType = FILTER_NODE_TYPE_UNDEFINED;
      }

      else if (strcasecmp(psXMLNode->pszValue, "PropertyIsNil") == 0) {
        const char* pszPropertyName = FLTGetPropertyName(psXMLNode);
        if( pszPropertyName != NULL )
        {
            psFilterNode->psLeftNode = FLTCreateFilterEncodingNode();
            psFilterNode->psLeftNode->pszValue = msStrdup(pszPropertyName);
            psFilterNode->psLeftNode->eType = FILTER_NODE_TYPE_PROPERTYNAME;
        }  else
          psFilterNode->eType = FILTER_NODE_TYPE_UNDEFINED;
      }
    }
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
    else if (FLTIsFeatureIdFilterType(psXMLNode->pszValue)) {
      psFilterNode->eType = FILTER_NODE_TYPE_FEATUREID;
      pszFeatureId = CPLGetXMLValue(psXMLNode, "fid", NULL);
       
      if (pszFeatureId == NULL)
        pszFeatureId = CPLGetXMLValue(psXMLNode, "id", NULL);
       
      if (pszFeatureId == NULL)
        pszFeatureId = CPLGetXMLValue(psXMLNode, "rid", NULL);
      pszFeatureIdList = NULL;

      psFeatureIdNode = psXMLNode;
      while (psFeatureIdNode) {
        pszFeatureId = CPLGetXMLValue(psFeatureIdNode, "fid", NULL);
        if (!pszFeatureId)
          pszFeatureId = CPLGetXMLValue(psFeatureIdNode, "id", NULL);
        if (!pszFeatureId)
          pszFeatureId = CPLGetXMLValue(psFeatureIdNode, "rid", NULL);

        if (pszFeatureId) {
          if (pszFeatureIdList)
            pszFeatureIdList = msStringConcatenate(pszFeatureIdList, ",");

          pszFeatureIdList = msStringConcatenate(pszFeatureIdList, pszFeatureId);
        }
        psFeatureIdNode = psFeatureIdNode->psNext;
      }

      if (pszFeatureIdList) {
        msFree(psFilterNode->pszValue);
        psFilterNode->pszValue =  msStrdup(pszFeatureIdList);
        msFree(pszFeatureIdList);
      } else
        psFilterNode->eType = FILTER_NODE_TYPE_UNDEFINED;
    }
    
     
     
     
     
    else if (FLTIsTemporalFilterType(psXMLNode->pszValue)) {
      psFilterNode->eType = FILTER_NODE_TYPE_TEMPORAL;

      if (strcasecmp(psXMLNode->pszValue, "During") == 0) {
        const char* pszPropertyName = NULL;
        const char* pszBeginTime;
        const char* pszEndTime;

        pszPropertyName = FLTGetPropertyName(psXMLNode);
        pszBeginTime = CPLGetXMLValue(psXMLNode, "TimePeriod.begin.TimeInstant.timePosition", NULL);
        if( pszBeginTime == NULL )
            pszBeginTime = CPLGetXMLValue(psXMLNode, "TimePeriod.beginPosition", NULL);
        pszEndTime = CPLGetXMLValue(psXMLNode, "TimePeriod.end.TimeInstant.timePosition", NULL);
        if( pszEndTime == NULL )
            pszEndTime = CPLGetXMLValue(psXMLNode, "TimePeriod.endPosition", NULL);

        if (pszPropertyName && pszBeginTime && pszEndTime &&
            strchr(pszBeginTime, '\'') == NULL && strchr(pszBeginTime, '\\') == NULL &&
            strchr(pszEndTime, '\'') == NULL && strchr(pszEndTime, '\\') == NULL &&
            msTimeGetResolution(pszBeginTime) >= 0 &&
            msTimeGetResolution(pszEndTime) >= 0) {

          psFilterNode->psLeftNode = FLTCreateFilterEncodingNode();
          psFilterNode->psLeftNode->eType =  FILTER_NODE_TYPE_PROPERTYNAME;
          psFilterNode->psLeftNode->pszValue = msStrdup(pszPropertyName);

          psFilterNode->psRightNode = FLTCreateFilterEncodingNode();
          psFilterNode->psRightNode->eType = FILTER_NODE_TYPE_TIME_PERIOD;
          psFilterNode->psRightNode->pszValue = msSmallMalloc( strlen(pszBeginTime) + strlen(pszEndTime) + 2 );
          sprintf(psFilterNode->psRightNode->pszValue, "%s/%s", pszBeginTime, pszEndTime);
        }
        else
          psFilterNode->eType = FILTER_NODE_TYPE_UNDEFINED;
      } else {
        psFilterNode->eType = FILTER_NODE_TYPE_UNDEFINED;
      }

    } 



  }
}
