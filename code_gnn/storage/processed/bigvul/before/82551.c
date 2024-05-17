Pin jshGetPinFromVar(
    JsVar *pinv  
  ) {
  if (jsvIsString(pinv) && pinv->varData.str[5]==0 ) {
    return jshGetPinFromString(&pinv->varData.str[0]);
  } else if (jsvIsInt(pinv)  ) {
    return (Pin)jsvGetInteger(pinv);
  } else return PIN_UNDEFINED;
}
