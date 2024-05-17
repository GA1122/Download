NO_INLINE JsVar *jspeFactorFunctionCall() {
   
  bool isConstructor = false;
  if (lex->tk==LEX_R_NEW) {
    JSP_ASSERT_MATCH(LEX_R_NEW);
    isConstructor = true;

    if (lex->tk==LEX_R_NEW) {
      jsExceptionHere(JSET_ERROR, "Nesting 'new' operators is unsupported");
      jspSetError(false);
      return 0;
    }
  }

  JsVar *parent = 0;
#ifndef SAVE_ON_FLASH
  bool wasSuper = lex->tk==LEX_R_SUPER;
#endif
  JsVar *a = jspeFactorMember(jspeFactor(), &parent);
#ifndef SAVE_ON_FLASH
  if (wasSuper) {
     
    jsvUnLock(parent);
    parent = jsvLockAgainSafe(execInfo.thisVar);
  }
#endif

  while ((lex->tk=='(' || (isConstructor && JSP_SHOULD_EXECUTE)) && !jspIsInterrupted()) {
    JsVar *funcName = a;
    JsVar *func = jsvSkipName(funcName);

     
    if (isConstructor && JSP_SHOULD_EXECUTE) {
      bool parseArgs = lex->tk=='(';
      a = jspeConstruct(func, funcName, parseArgs);
      isConstructor = false;  
    } else
      a = jspeFunctionCall(func, funcName, parent, true, 0, 0);

    jsvUnLock3(funcName, func, parent);
    parent=0;
    a = jspeFactorMember(a, &parent);
  }
#ifndef SAVE_ON_FLASH
   
  if (parent && jsvIsName(a) && !jsvIsNewChild(a)) {
    JsVar *value = jsvGetValueOfName(a);
    if (jsvIsGetterOrSetter(value)) {  
      JsVar *nameVar = jsvCopyNameOnly(a,false,true);
      JsVar *newChild = jsvCreateNewChild(parent, nameVar, value);
      jsvUnLock2(nameVar, a);
      a = newChild;
    }
    jsvUnLock(value);
  }
#endif
  jsvUnLock(parent);
  return a;
}
