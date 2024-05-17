PPB_Flash_MessageLoop_Impl::PPB_Flash_MessageLoop_Impl(PP_Instance instance)
    : Resource(ppapi::OBJECT_IS_IMPL, instance), state_(new State()) {}
