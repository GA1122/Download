void *jsvMalloc(size_t size) {
   
  JsVar *flatStr = jsvNewFlatStringOfLength((unsigned int)size);
  if (!flatStr) {
    jsErrorFlags |= JSERR_LOW_MEMORY;
    while (jsiFreeMoreMemory());
    jsvGarbageCollect();
    flatStr = jsvNewFlatStringOfLength((unsigned int)size);
  }
  void *p = (void*)jsvGetFlatStringPointer(flatStr);
  if (p) {
    memset(p,0,size);
  }
  return p;
}
