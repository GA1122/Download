void *addDeferredMultiBulkLength(client *c) {
     
    if (prepareClientToWrite(c) != C_OK) return NULL;
    listAddNodeTail(c->reply,createObject(OBJ_STRING,NULL));
    return listLast(c->reply);
}
