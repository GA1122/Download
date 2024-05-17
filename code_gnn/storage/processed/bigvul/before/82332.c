NO_INLINE JsVar *jspeFunctionDefinition(bool parseNamedFunction) {
  JsVar *funcVar = 0;

  bool actuallyCreateFunction = JSP_SHOULD_EXECUTE;
  if (actuallyCreateFunction)
    funcVar = jsvNewWithFlags(JSV_FUNCTION);

  JsVar *functionInternalName = 0;
  if (parseNamedFunction && lex->tk==LEX_ID) {
    if (funcVar) functionInternalName = jslGetTokenValueAsVar(lex);
    JSP_ASSERT_MATCH(LEX_ID);
  }

  if (!jspeFunctionArguments(funcVar)) {
    jsvUnLock2(functionInternalName, funcVar);
    return 0;
  }

  jspeFunctionDefinitionInternal(funcVar, false);

  if (funcVar && functionInternalName)
    jsvObjectSetChildAndUnLock(funcVar, JSPARSE_FUNCTION_NAME_NAME, functionInternalName);

  return funcVar;
}
