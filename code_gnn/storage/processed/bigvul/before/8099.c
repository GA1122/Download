GBool GfxResources::lookupGState(char *name, Object *obj) {
  if (!lookupGStateNF(name, obj))
    return gFalse;

  if (!obj->isRef())
    return gTrue;
  
  const Ref ref = obj->getRef();
  if (!gStateCache.lookup(ref, obj)->isNull())
    return gTrue;
  obj->free();

  gStateCache.put(ref)->copy(obj);
  return gTrue;
}
