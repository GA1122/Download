__xmlFree(void){
    if (IS_MAIN_THREAD)
        return (&xmlFree);
    else
        return (&xmlGetGlobalState()->xmlFree);
}
