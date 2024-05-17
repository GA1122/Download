NO_INLINE JsVar *__jspeBinaryExpression(JsVar *a, unsigned int lastPrecedence) {
   
  unsigned int precedence = jspeGetBinaryExpressionPrecedence(lex->tk);
  while (precedence && precedence>lastPrecedence) {
    int op = lex->tk;
    JSP_ASSERT_MATCH(op);

    if (op==LEX_ANDAND || op==LEX_OROR) {
      bool aValue = jsvGetBoolAndUnLock(jsvSkipName(a));
      if ((!aValue && op==LEX_ANDAND) ||
          (aValue && op==LEX_OROR)) {
        JSP_SAVE_EXECUTE();
        jspSetNoExecute();
        jsvUnLock(__jspeBinaryExpression(jspeUnaryExpression(),precedence));
        JSP_RESTORE_EXECUTE();
      } else {
        jsvUnLock(a);
        a = __jspeBinaryExpression(jspeUnaryExpression(),precedence);
      }
    } else {  
      JsVar *b = __jspeBinaryExpression(jspeUnaryExpression(),precedence);
      if (JSP_SHOULD_EXECUTE) {
        if (op==LEX_R_IN) {
          JsVar *av = jsvSkipName(a);  
          JsVar *bv = jsvSkipName(b);  
          if (jsvIsArray(bv) || jsvIsObject(bv)) {  
            av = jsvAsArrayIndexAndUnLock(av);
            JsVar *varFound = jspGetVarNamedField( bv, av, true);
            jsvUnLock(a);
            a = jsvNewFromBool(varFound!=0);
            jsvUnLock(varFound);
          } else { 
            jsExceptionHere(JSET_ERROR, "Cannot use 'in' operator to search a %t", bv);
            jsvUnLock(a);
            a = 0;
          }
          jsvUnLock2(av, bv);
        } else if (op==LEX_R_INSTANCEOF) {
          bool inst = false;
          JsVar *av = jsvSkipName(a);
          JsVar *bv = jsvSkipName(b);
          if (!jsvIsFunction(bv)) {
            jsExceptionHere(JSET_ERROR, "Expecting a function on RHS in instanceof check, got %t", bv);
          } else {
            if (jsvIsObject(av) || jsvIsFunction(av)) {
              JsVar *bproto = jspGetNamedField(bv, JSPARSE_PROTOTYPE_VAR, false);
              JsVar *proto = jsvObjectGetChild(av, JSPARSE_INHERITS_VAR, 0);
              while (proto) {
                if (proto == bproto) inst=true;
                JsVar *childProto = jsvObjectGetChild(proto, JSPARSE_INHERITS_VAR, 0);
                jsvUnLock(proto);
                proto = childProto;
              }
              if (jspIsConstructor(bv, "Object")) inst = true;
              jsvUnLock(bproto);
            }
            if (!inst) {
              const char *name = jswGetBasicObjectName(av);
              if (name) {
                inst = jspIsConstructor(bv, name);
              }
              if (!inst && (jsvIsArray(av) || jsvIsArrayBuffer(av)) &&
                  jspIsConstructor(bv, "Object"))
                inst = true;
            }
          }
          jsvUnLock3(av, bv, a);
          a = jsvNewFromBool(inst);
        } else {   
          JsVar *res = jsvMathsOpSkipNames(a, b, op);
          jsvUnLock(a); a = res;
        }
      }
      jsvUnLock(b);
    }
    precedence = jspeGetBinaryExpressionPrecedence(lex->tk);
  }
  return a;
}
