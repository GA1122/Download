JsVar *jsvAsArrayIndex(JsVar *index) {
  if (jsvIsSimpleInt(index)) {
    return jsvLockAgain(index);  
  } else if (jsvIsString(index)) {
     
    if (jsvIsStringNumericStrict(index)) {
      JsVar *i = jsvNewFromInteger(jsvGetInteger(index));
      JsVar *is = jsvAsString(i, false);
      if (jsvCompareString(index,is,0,0,false)==0) {
        jsvUnLock(is);
        return i;
      } else {
        jsvUnLock2(i,is);
      }
    }
  } else if (jsvIsFloat(index)) {
    JsVarFloat v = jsvGetFloat(index);
    JsVarInt vi = jsvGetInteger(index);
    if (v == vi) return jsvNewFromInteger(vi);
  }

  return jsvAsString(index, false);
}
