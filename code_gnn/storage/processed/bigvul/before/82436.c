void *jsvGetNativeFunctionPtr(const JsVar *function) {
   
  JsVar *flatString = jsvFindChildFromString((JsVar*)function, JSPARSE_FUNCTION_CODE_NAME, 0);
  if (flatString) {
    flatString = jsvSkipNameAndUnLock(flatString);
    void *v = (void*)((size_t)function->varData.native.ptr + (char*)jsvGetFlatStringPointer(flatString));
    jsvUnLock(flatString);
    return v;
  } else
    return (void *)function->varData.native.ptr;
}
