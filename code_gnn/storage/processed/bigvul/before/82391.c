void jsvFree(void *ptr) {
  JsVar *flatStr = jsvGetFlatStringFromPointer((char *)ptr);

  jsvUnLock(flatStr);
}
