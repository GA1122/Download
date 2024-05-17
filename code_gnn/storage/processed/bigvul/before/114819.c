void xmlInitGlobals(void)
{
    if (xmlThrDefMutex == NULL)
        xmlThrDefMutex = xmlNewMutex();
}
