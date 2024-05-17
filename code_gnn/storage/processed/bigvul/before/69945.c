void addReplySds(client *c, sds s) {
    if (prepareClientToWrite(c) != C_OK) {
         
        sdsfree(s);
        return;
    }
    if (_addReplyToBuffer(c,s,sdslen(s)) == C_OK) {
        sdsfree(s);
    } else {
         
        _addReplySdsToList(c,s);
    }
}
