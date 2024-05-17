JsVar *jspExecuteFunction(JsVar *func, JsVar *thisArg, int argCount, JsVar **argPtr) {
  JsExecInfo oldExecInfo = execInfo;

  execInfo.scopeCount = 0;
  execInfo.execute = EXEC_YES;
  execInfo.thisVar = 0;
  JsVar *result = jspeFunctionCall(func, 0, thisArg, false, argCount, argPtr);
  assert(execInfo.scopeCount==0);
  oldExecInfo.execute = execInfo.execute;  
  execInfo = oldExecInfo;

  return result;
}
