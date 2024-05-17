NO_INLINE JsVar *jspNewObject(const char *name, const char *instanceOf) {
  JsVar *prototypeName = jspNewPrototype(instanceOf);

  JsVar *obj = jsvNewObject();
  if (!obj) {  
    jsvUnLock(prototypeName);
    return 0;
  }
  if (name) {
    IOEventFlags device = jshFromDeviceString(name);
    if (device!=EV_NONE) {
      obj->varData.str[0] = 'D';
      obj->varData.str[1] = 'E';
      obj->varData.str[2] = 'V';
      obj->varData.str[3] = (char)device;
    }

  }
  JsVar *prototypeVar = jsvSkipName(prototypeName);
  jsvUnLock3(jsvAddNamedChild(obj, prototypeVar, JSPARSE_INHERITS_VAR), prototypeVar, prototypeName);prototypeName=0;

  if (name) {
    JsVar *objName = jsvFindChildFromString(execInfo.root, name, true);
    if (objName) jsvSetValueOfName(objName, obj);
    jsvUnLock(obj);
    if (!objName) {  
      return 0;
    }
    return objName;
  } else
    return obj;
}