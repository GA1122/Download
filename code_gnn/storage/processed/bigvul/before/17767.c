add_master(ClientPtr client, xXIAddMasterInfo * c, int flags[MAXDEVICES])
{
    DeviceIntPtr ptr, keybd, XTestptr, XTestkeybd;
    char *name;
    int rc;

    name = calloc(c->name_len + 1, sizeof(char));
    if (name == NULL) {
        rc = BadAlloc;
        goto unwind;
    }
    strncpy(name, (char *) &c[1], c->name_len);

    rc = AllocDevicePair(client, name, &ptr, &keybd,
                         CorePointerProc, CoreKeyboardProc, TRUE);
    if (rc != Success)
        goto unwind;

    if (!c->send_core)
        ptr->coreEvents = keybd->coreEvents = FALSE;

     
    rc = AllocXTestDevice(client, name, &XTestptr, &XTestkeybd, ptr, keybd);
    if (rc != Success) {
        DeleteInputDeviceRequest(ptr);
        DeleteInputDeviceRequest(keybd);
        goto unwind;
    }

    ActivateDevice(ptr, FALSE);
    ActivateDevice(keybd, FALSE);
    flags[ptr->id] |= XIMasterAdded;
    flags[keybd->id] |= XIMasterAdded;

    ActivateDevice(XTestptr, FALSE);
    ActivateDevice(XTestkeybd, FALSE);
    flags[XTestptr->id] |= XISlaveAdded;
    flags[XTestkeybd->id] |= XISlaveAdded;

    if (c->enable) {
        EnableDevice(ptr, FALSE);
        EnableDevice(keybd, FALSE);
        flags[ptr->id] |= XIDeviceEnabled;
        flags[keybd->id] |= XIDeviceEnabled;

        EnableDevice(XTestptr, FALSE);
        EnableDevice(XTestkeybd, FALSE);
        flags[XTestptr->id] |= XIDeviceEnabled;
        flags[XTestkeybd->id] |= XIDeviceEnabled;
    }

     
    AttachDevice(NULL, XTestptr, ptr);
    AttachDevice(NULL, XTestkeybd, keybd);
    flags[XTestptr->id] |= XISlaveAttached;
    flags[XTestkeybd->id] |= XISlaveAttached;

    for (int i = 0; i < currentMaxClients; i++)
        XIBarrierNewMasterDevice(clients[i], ptr->id);

 unwind:
    free(name);
    return rc;
}
