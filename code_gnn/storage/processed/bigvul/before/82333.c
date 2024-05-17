NO_INLINE bool jspeFunctionDefinitionInternal(JsVar *funcVar, bool expressionOnly) {
  if (expressionOnly) {
    if (funcVar)
      funcVar->flags = (funcVar->flags & ~JSV_VARTYPEMASK) | JSV_FUNCTION_RETURN;
  } else {
    JSP_MATCH('{');

  #ifndef SAVE_ON_FLASH
    if (lex->tk==LEX_STR && !strcmp(jslGetTokenValueAsString(lex), "compiled")) {
      jsWarn("Function marked with \"compiled\" uploaded in source form");
    }
  #endif

     
    if (funcVar && lex->tk==LEX_R_RETURN) {
      funcVar->flags = (funcVar->flags & ~JSV_VARTYPEMASK) | JSV_FUNCTION_RETURN;
      JSP_ASSERT_MATCH(LEX_R_RETURN);
    }
  }
  JsVarInt lineNumber = 0;
  if (funcVar && lex->lineNumberOffset) {
    lineNumber = (JsVarInt)jslGetLineNumber(lex) + (JsVarInt)lex->lineNumberOffset - 1;
  }
  JslCharPos funcBegin = jslCharPosClone(&lex->tokenStart);
  int lastTokenEnd = -1;
  if (!expressionOnly) {
    int brackets = 0;
    while (lex->tk && (brackets || lex->tk != '}')) {
      if (lex->tk == '{') brackets++;
      if (lex->tk == '}') brackets--;
      lastTokenEnd = (int)jsvStringIteratorGetIndex(&lex->it)-1;
      JSP_ASSERT_MATCH(lex->tk);
    }
  } else {
    JsExecFlags oldExec = execInfo.execute;
    execInfo.execute = EXEC_NO;
    jsvUnLock(jspeAssignmentExpression());
    execInfo.execute = oldExec;
    lastTokenEnd = (int)jsvStringIteratorGetIndex(&lex->tokenStart.it)-1;
  }
  if (funcVar && lastTokenEnd>0) {
    JsVar *funcCodeVar;
    if (jsvIsNativeString(lex->sourceVar)) {
       
      int s = (int)jsvStringIteratorGetIndex(&funcBegin.it) - 1;
      funcCodeVar = jsvNewNativeString(lex->sourceVar->varData.nativeStr.ptr + s, (unsigned int)(lastTokenEnd - s));
    } else {
      if (jsfGetFlag(JSF_PRETOKENISE)) {
        funcCodeVar = jslNewTokenisedStringFromLexer(&funcBegin, (size_t)lastTokenEnd);
      } else {
        funcCodeVar = jslNewStringFromLexer(&funcBegin, (size_t)lastTokenEnd);
      }
    }
    jsvUnLock2(jsvAddNamedChild(funcVar, funcCodeVar, JSPARSE_FUNCTION_CODE_NAME), funcCodeVar);
    JsVar *funcScopeVar = jspeiGetScopesAsVar();
    if (funcScopeVar) {
      jsvUnLock2(jsvAddNamedChild(funcVar, funcScopeVar, JSPARSE_FUNCTION_SCOPE_NAME), funcScopeVar);
    }
    if (lineNumber) {
      JsVar *funcLineNumber = jsvNewFromInteger(lineNumber);
      if (funcLineNumber) {
        jsvUnLock2(jsvAddNamedChild(funcVar, funcLineNumber, JSPARSE_FUNCTION_LINENUMBER_NAME), funcLineNumber);
      }
    }
  }

  jslCharPosFree(&funcBegin);
  if (!expressionOnly) JSP_MATCH('}');
