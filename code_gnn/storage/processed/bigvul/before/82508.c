bool jsvMathsOpTypeEqual(JsVar *a, JsVar *b) {
  bool eql = (a==0) == (b==0);
  if (a && b) {
    eql = ((jsvIsInt(a)||jsvIsFloat(a)) && (jsvIsInt(b)||jsvIsFloat(b))) ||
        ((a->flags & JSV_VARTYPEMASK) == (b->flags & JSV_VARTYPEMASK));
  }
  if (eql) {
    JsVar *contents = jsvMathsOp(a,b, LEX_EQUAL);
    if (!jsvGetBool(contents)) eql = false;
    jsvUnLock(contents);
  } else {
     
    assert(!(jsvIsString(a) && jsvIsString(b) && jsvIsBasicVarEqual(a,b)));
  }
  return eql;
}
