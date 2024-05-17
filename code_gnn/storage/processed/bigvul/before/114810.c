__xmlRealloc(void){
    if (IS_MAIN_THREAD)
        return (&xmlRealloc);
    else
        return (&xmlGetGlobalState()->xmlRealloc);
}
