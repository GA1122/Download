PickPointer(ClientPtr client)
{
    DeviceIntPtr it = inputInfo.devices;

     
    for (it = inputInfo.devices; it; it = it->next) {
        GrabPtr grab = it->deviceGrab.grab;

        if (grab && grab->grabtype == CORE && SameClient(grab, client)) {
            it = GetMaster(it, MASTER_POINTER);
            return it;           
        }
    }

    if (!client->clientPtr) {
        it = inputInfo.devices;
        while (it) {
            if (IsMaster(it) && it->spriteInfo->spriteOwner) {
                client->clientPtr = it;
                break;
            }
            it = it->next;
        }
    }
    return client->clientPtr;
}
