JsVar *jsvAsName(JsVar *var) {
  if (!var) return 0;
  if (jsvGetRefs(var) == 0) {
    if (!jsvIsName(var))
      var = jsvMakeIntoVariableName(var, 0);
    return jsvLockAgain(var);
  } else {  
    return jsvMakeIntoVariableName(jsvCopy(var, false), 0);
  }
}
