JsVar *jspEvaluateVar(JsVar *str, JsVar *scope, uint16_t lineNumberOffset) {
  JsLex lex;

  assert(jsvIsString(str));
  JsLex *oldLex = jslSetLex(&lex);
  jslInit(str);
  lex.lineNumberOffset = lineNumberOffset;


  JsExecInfo oldExecInfo = execInfo;
  execInfo.execute = EXEC_YES;
  bool scopeAdded = false;
  if (scope) {
    execInfo.scopeCount = 0;
    scopeAdded = jspeiAddScope(scope);
  }

  JsVar *v = jspParse();
  if (scopeAdded)
    jspeiRemoveScope();
  jslKill();
  jslSetLex(oldLex);

  JsExecFlags mask = EXEC_FOR_INIT|EXEC_IN_LOOP|EXEC_IN_SWITCH;
  oldExecInfo.execute = (oldExecInfo.execute & mask) | (execInfo.execute & ~mask); 
  execInfo = oldExecInfo;

  return jsvSkipNameAndUnLock(v);
}
