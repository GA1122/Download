NO_INLINE JsVar *jspeFactorMember(JsVar *a, JsVar **parentResult) {
   
  JsVar *parent = 0;

  while (lex->tk=='.' || lex->tk=='[') {
    if (lex->tk == '.') {  
      JSP_ASSERT_MATCH('.');
      if (jslIsIDOrReservedWord(lex)) {
        if (JSP_SHOULD_EXECUTE) {
          const char *name = jslGetTokenValueAsString(lex);

          JsVar *aVar = jsvSkipName(a);
          JsVar *child = 0;
          if (aVar)
            child = jspGetNamedField(aVar, name, true);
          if (!child) {
            if (!jsvIsUndefined(aVar)) {
              JsVar *nameVar = jslGetTokenValueAsVar(lex);
              child = jsvCreateNewChild(aVar, nameVar, 0);
              jsvUnLock(nameVar);
            } else {
              jsExceptionHere(JSET_ERROR, "Cannot read property '%s' of undefined", name);
            }
          }
          jsvUnLock(parent);
          parent = aVar;
          jsvUnLock(a);
          a = child;
        }
        jslGetNextToken(lex);
      } else {
        JSP_MATCH_WITH_RETURN(LEX_ID, a);
      }
    } else if (lex->tk == '[') {  
      JsVar *index;
      JSP_ASSERT_MATCH('[');
      if (!jspCheckStackPosition()) return parent;
      index = jsvSkipNameAndUnLock(jspeAssignmentExpression());
      JSP_MATCH_WITH_CLEANUP_AND_RETURN(']', jsvUnLock2(parent, index);, a);
      if (JSP_SHOULD_EXECUTE) {
        index = jsvAsArrayIndexAndUnLock(index);
        JsVar *aVar = jsvSkipName(a);
        JsVar *child = 0;
        if (aVar)
          child = jspGetVarNamedField(aVar, index, true);

        if (!child) {
          if (jsvHasChildren(aVar)) {
            child = jsvCreateNewChild(aVar, index, 0);
          } else {
            jsExceptionHere(JSET_ERROR, "Field or method %q does not already exist, and can't create it on %t", index, aVar);
          }
        }
        jsvUnLock(parent);
        parent = jsvLockAgainSafe(aVar);
        jsvUnLock(a);
        a = child;
        jsvUnLock(aVar);
      }
      jsvUnLock(index);
    } else {
      assert(0);
    }
  }

  if (parentResult) *parentResult = parent;
  else jsvUnLock(parent);
  return a;
}
