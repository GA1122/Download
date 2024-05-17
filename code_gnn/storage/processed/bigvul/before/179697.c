 int msPostGISLayerSetTimeFilter(layerObj *lp, const char *timestring, const char *timefield)
 {
   char **atimes, **aranges = NULL;
   int numtimes=0,i=0,numranges=0;
   size_t buffer_size = 512;
   char buffer[512], bufferTmp[512];
 
   buffer[0] = '\0';
   bufferTmp[0] = '\0';
 
    if (!lp || !timestring || !timefield)
      return MS_FALSE;
  
//   if( strchr(timestring,'\'') || strchr(timestring, '\\') ) {
//      msSetError(MS_MISCERR, "Invalid time filter.", "msPostGISLayerSetTimeFilter()");
//      return MS_FALSE;
//   }
// 
     
    if (strstr(timestring, ",") == NULL &&
        strstr(timestring, "/") == NULL) {  
     createPostgresTimeCompareSimple(timefield, timestring, buffer, buffer_size);
   } else {
 
      
     atimes = msStringSplit (timestring, ',', &numtimes);
     if (atimes == NULL || numtimes < 1)
       return MS_FALSE;
 
     strlcat(buffer, "(", buffer_size);
     for(i=0; i<numtimes; i++) {
       if(i!=0) {
         strlcat(buffer, " OR ", buffer_size);
       }
       strlcat(buffer, "(", buffer_size);
       aranges = msStringSplit(atimes[i],  '/', &numranges);
       if(!aranges) return MS_FALSE;
       if(numranges == 1) {
          
         createPostgresTimeCompareSimple(timefield, atimes[i], bufferTmp, buffer_size);
         strlcat(buffer, bufferTmp, buffer_size);
       } else if(numranges == 2) {
          
         createPostgresTimeCompareRange(timefield, aranges[0], aranges[1], bufferTmp, buffer_size);
         strlcat(buffer, bufferTmp, buffer_size);
       } else {
         return MS_FALSE;
       }
       msFreeCharArray(aranges, numranges);
       strlcat(buffer, ")", buffer_size);
     }
     strlcat(buffer, ")", buffer_size);
     msFreeCharArray(atimes, numtimes);
   }
   if(!*buffer) {
     return MS_FALSE;
   }
   if(lp->filteritem) free(lp->filteritem);
   lp->filteritem = msStrdup(timefield);
   if (&lp->filter) {
      
     if (lp->filter.type == MS_EXPRESSION) {
       snprintf(bufferTmp, buffer_size, "(%s) and %s", lp->filter.string, buffer);
       loadExpressionString(&lp->filter, bufferTmp);
     } else {
       freeExpression(&lp->filter);
       loadExpressionString(&lp->filter, buffer);
     }
   }
 
 
   return MS_TRUE;
 }