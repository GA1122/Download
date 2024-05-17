xmlBufferAllocationScheme xmlThrDefBufferAllocScheme(xmlBufferAllocationScheme v) {
    xmlBufferAllocationScheme ret;
    xmlMutexLock(xmlThrDefMutex);
    ret = xmlBufferAllocSchemeThrDef;
    xmlBufferAllocSchemeThrDef = v;
    xmlMutexUnlock(xmlThrDefMutex);
    return ret;
}
