size_t jsvGetStringLength(const JsVar *v) {
  size_t strLength = 0;
  const JsVar *var = v;
  JsVar *newVar = 0;
  if (!jsvHasCharacterData(v)) return 0;

  while (var) {
    JsVarRef ref = jsvGetLastChild(var);
    strLength += jsvGetCharactersInVar(var);

    jsvUnLock(newVar);  
    var = newVar = ref ? jsvLock(ref) : 0;
  }
  jsvUnLock(newVar);  
  return strLength;
}
