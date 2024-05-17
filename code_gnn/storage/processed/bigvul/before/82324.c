NO_INLINE JsVar *jspeFactorArray() {
  int idx = 0;
  JsVar *contents = 0;
  if (JSP_SHOULD_EXECUTE) {
    contents = jsvNewEmptyArray();
    if (!contents) {  
      jspSetError(false);
      return 0;
    }
  }
   
  JSP_MATCH_WITH_RETURN('[', contents);
  while (!JSP_SHOULDNT_PARSE && lex->tk != ']') {
    if (JSP_SHOULD_EXECUTE) {
      JsVar *aVar = 0;
      JsVar *indexName = 0;
      if (lex->tk != ',') {  
        aVar = jsvSkipNameAndUnLock(jspeAssignmentExpression());
        indexName = jsvMakeIntoVariableName(jsvNewFromInteger(idx),  aVar);
      }
      if (indexName) {  
        jsvAddName(contents, indexName);
        jsvUnLock(indexName);
      }
      jsvUnLock(aVar);
    } else {
      jsvUnLock(jspeAssignmentExpression());
    }
    if (lex->tk != ']') JSP_MATCH_WITH_RETURN(',', contents);
    idx++;
  }
  if (contents) jsvSetArrayLength(contents, idx, false);
  JSP_MATCH_WITH_RETURN(']', contents);
  return contents;
}
