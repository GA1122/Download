__xmlMallocAtomic(void){
    if (IS_MAIN_THREAD)
        return (&xmlMallocAtomic);
    else
        return (&xmlGetGlobalState()->xmlMallocAtomic);
}
