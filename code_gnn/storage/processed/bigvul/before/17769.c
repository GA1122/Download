detach_slave(ClientPtr client, xXIDetachSlaveInfo * c, int flags[MAXDEVICES])
{
    DeviceIntPtr dev;
    int rc;

    rc = dixLookupDevice(&dev, c->deviceid, client, DixManageAccess);
    if (rc != Success)
        goto unwind;

    if (IsMaster(dev)) {
        client->errorValue = c->deviceid;
        rc = BadDevice;
        goto unwind;
    }

     
    if (IsXTestDevice(dev, NULL)) {
        client->errorValue = c->deviceid;
        rc = BadDevice;
        goto unwind;
    }

    ReleaseButtonsAndKeys(dev);
    AttachDevice(client, dev, NULL);
    flags[dev->id] |= XISlaveDetached;

 unwind:
    return rc;
}
