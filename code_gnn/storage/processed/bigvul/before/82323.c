NO_INLINE JsVar *jspeExpressionOrArrowFunction() {
  JsVar *a = 0;
  JsVar *funcVar = 0;
  bool allNames = true;
  while (lex->tk!=')' && !JSP_SHOULDNT_PARSE) {
    if (allNames && a) {
      funcVar = jspeAddNamedFunctionParameter(funcVar, a);
    }
    jsvUnLock(a);
    a = jspeAssignmentExpression();
    if (!(jsvIsName(a) && jsvIsString(a))) allNames = false;
    if (lex->tk!=')') JSP_MATCH_WITH_CLEANUP_AND_RETURN(',', jsvUnLock2(a,funcVar), 0);
  }
  JSP_MATCH_WITH_CLEANUP_AND_RETURN(')', jsvUnLock2(a,funcVar), 0);
  if (allNames && lex->tk==LEX_ARROW_FUNCTION) {
    funcVar = jspeArrowFunction(funcVar, a);
    jsvUnLock(a);
    return funcVar;
  } else {
    jsvUnLock(funcVar);
    return a;
  }
}
