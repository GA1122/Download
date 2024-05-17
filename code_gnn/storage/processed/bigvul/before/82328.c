NO_INLINE JsVar *jspeFactorObject() {
  if (JSP_SHOULD_EXECUTE) {
    JsVar *contents = jsvNewObject();
    if (!contents) {  
      jspSetError(false);
      return 0;
    }
     
    JSP_MATCH_WITH_RETURN('{', contents);
    while (!JSP_SHOULDNT_PARSE && lex->tk != '}') {
      JsVar *varName = 0;
      if (jslIsIDOrReservedWord(lex)) {
        if (JSP_SHOULD_EXECUTE)
          varName = jslGetTokenValueAsVar(lex);
        jslGetNextToken(lex);  
      } else if (
          lex->tk==LEX_STR ||
          lex->tk==LEX_TEMPLATE_LITERAL ||
          lex->tk==LEX_FLOAT ||
          lex->tk==LEX_INT ||
          lex->tk==LEX_R_TRUE ||
          lex->tk==LEX_R_FALSE ||
          lex->tk==LEX_R_NULL ||
          lex->tk==LEX_R_UNDEFINED) {
        varName = jspeFactor();
      } else {
        JSP_MATCH_WITH_RETURN(LEX_ID, contents);
      }
      JSP_MATCH_WITH_CLEANUP_AND_RETURN(':', jsvUnLock(varName), contents);
      if (JSP_SHOULD_EXECUTE) {
        varName = jsvAsArrayIndexAndUnLock(varName);
        JsVar *contentsName = jsvFindChildFromVar(contents, varName, true);
        if (contentsName) {
          JsVar *value = jsvSkipNameAndUnLock(jspeAssignmentExpression());  
          jsvUnLock2(jsvSetValueOfName(contentsName, value), value);
        }
      }
      jsvUnLock(varName);
      if (lex->tk != '}') JSP_MATCH_WITH_RETURN(',', contents);
    }
    JSP_MATCH_WITH_RETURN('}', contents);