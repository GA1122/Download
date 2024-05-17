NO_INLINE JsVar *jspeClassDefinition(bool parseNamedClass) {
  JsVar *classFunction = 0;
  JsVar *classPrototype = 0;
  JsVar *classInternalName = 0;

  bool actuallyCreateClass = JSP_SHOULD_EXECUTE;
  if (actuallyCreateClass)
    classFunction = jsvNewWithFlags(JSV_FUNCTION);

  if (parseNamedClass && lex->tk==LEX_ID) {
    if (classFunction)
      classInternalName = jslGetTokenValueAsVar(lex);
    JSP_ASSERT_MATCH(LEX_ID);
  }
  if (classFunction) {
    JsVar *prototypeName = jsvFindChildFromString(classFunction, JSPARSE_PROTOTYPE_VAR, true);
    jspEnsureIsPrototype(classFunction, prototypeName);  
    classPrototype = jsvSkipName(prototypeName);
    jsvUnLock(prototypeName);
  }
  if (lex->tk==LEX_R_EXTENDS) {
    JSP_ASSERT_MATCH(LEX_R_EXTENDS);
    JsVar *extendsFrom = actuallyCreateClass ? jsvSkipNameAndUnLock(jspGetNamedVariable(jslGetTokenValueAsString(lex))) : 0;
    JSP_MATCH_WITH_CLEANUP_AND_RETURN(LEX_ID,jsvUnLock4(extendsFrom,classFunction,classInternalName,classPrototype),0);
    if (classPrototype) {
      if (jsvIsFunction(extendsFrom)) {
        jsvObjectSetChild(classPrototype, JSPARSE_INHERITS_VAR, extendsFrom);
        jsvObjectSetChildAndUnLock(classFunction, JSPARSE_FUNCTION_CODE_NAME, jsvNewFromString("if(this.__proto__.__proto__)this.__proto__.__proto__.apply(this,arguments)"));
      } else
        jsExceptionHere(JSET_SYNTAXERROR, "'extends' argument should be a function, got %t", extendsFrom);
    }
    jsvUnLock(extendsFrom);
  }
  JSP_MATCH_WITH_CLEANUP_AND_RETURN('{',jsvUnLock3(classFunction,classInternalName,classPrototype),0);

  while ((lex->tk==LEX_ID || lex->tk==LEX_R_STATIC) && !jspIsInterrupted()) {
    bool isStatic = lex->tk==LEX_R_STATIC;
    if (isStatic) JSP_ASSERT_MATCH(LEX_R_STATIC);

    JsVar *funcName = jslGetTokenValueAsVar(lex);
    JSP_MATCH_WITH_CLEANUP_AND_RETURN(LEX_ID,jsvUnLock3(classFunction,classInternalName,classPrototype),0);
    JsVar *method = jspeFunctionDefinition(false);
    if (classFunction && classPrototype) {
      if (jsvIsStringEqual(funcName, "get") || jsvIsStringEqual(funcName, "set")) {
        jsExceptionHere(JSET_SYNTAXERROR, "'get' and 'set' and not supported in Espruino");
      } else if (jsvIsStringEqual(funcName, "constructor")) {
        jswrap_function_replaceWith(classFunction, method);
      } else {
        funcName = jsvMakeIntoVariableName(funcName, 0);
        jsvSetValueOfName(funcName, method);
        jsvAddName(isStatic ? classFunction : classPrototype, funcName);
      }
    }
    jsvUnLock2(method,funcName);
  }
  jsvUnLock(classPrototype);
  if (classInternalName)
    jsvObjectSetChildAndUnLock(classFunction, JSPARSE_FUNCTION_NAME_NAME, classInternalName);

  JSP_MATCH_WITH_CLEANUP_AND_RETURN('}',jsvUnLock(classFunction),0);
  return classFunction;
}