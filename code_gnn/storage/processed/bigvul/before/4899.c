WindowHasNewCursor(WindowPtr pWin)
{
    DeviceIntPtr pDev;

    for (pDev = inputInfo.devices; pDev; pDev = pDev->next)
        if (DevHasCursor(pDev))
            PostNewCursor(pDev);
}
