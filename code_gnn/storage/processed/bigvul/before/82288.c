JsVar *jspEvaluateModule(JsVar *moduleContents) {
  assert(jsvIsString(moduleContents) || jsvIsFunction(moduleContents));
  if (jsvIsFunction(moduleContents)) {
    moduleContents = jsvObjectGetChild(moduleContents,JSPARSE_FUNCTION_CODE_NAME,0);
    if (!jsvIsString(moduleContents)) {
      jsvUnLock(moduleContents);
      return 0;
    }
  } else
    jsvLockAgain(moduleContents);
  JsVar *scope = jsvNewObject();
  JsVar *scopeExports = jsvNewObject();
  if (!scope || !scopeExports) {  
    jsvUnLock3(scope, scopeExports, moduleContents);
    return 0;
  }
  JsVar *exportsName = jsvAddNamedChild(scope, scopeExports, "exports");
  jsvUnLock2(scopeExports, jsvAddNamedChild(scope, scope, "module"));

  JsExecFlags oldExecute = execInfo.execute;
  JsVar *oldThisVar = execInfo.thisVar;
  execInfo.thisVar = scopeExports;  
  jsvUnLock(jspEvaluateVar(moduleContents, scope, 0));
  execInfo.thisVar = oldThisVar;
  execInfo.execute = oldExecute;  

  jsvUnLock2(moduleContents, scope);
  return jsvSkipNameAndUnLock(exportsName);
}
