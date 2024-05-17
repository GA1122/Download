status_t OMX::listNodes(List<ComponentInfo> *list) {
 list->clear();

    OMX_U32 index = 0;
 char componentName[256];
 while (mMaster->enumerateComponents(
                componentName, sizeof(componentName), index) == OMX_ErrorNone) {
 list->push_back(ComponentInfo());
 ComponentInfo &info = *--list->end();

        info.mName = componentName;

 Vector<String8> roles;
        OMX_ERRORTYPE err =
            mMaster->getRolesOfComponent(componentName, &roles);

 if (err == OMX_ErrorNone) {
 for (OMX_U32 i = 0; i < roles.size(); ++i) {
                info.mRoles.push_back(roles[i]);
 }
 }

 ++index;
 }

 return OK;
}
