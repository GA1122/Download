void xmlCleanupGlobals(void)
{
    if (xmlThrDefMutex != NULL) {
	xmlFreeMutex(xmlThrDefMutex);
	xmlThrDefMutex = NULL;
    }
    __xmlGlobalInitMutexDestroy();
}
