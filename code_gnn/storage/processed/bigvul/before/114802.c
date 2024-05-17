__xmlMalloc(void){
    if (IS_MAIN_THREAD)
        return (&xmlMalloc);
    else
    	return (&xmlGetGlobalState()->xmlMalloc);
}
