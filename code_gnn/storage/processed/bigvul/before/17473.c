ProcXvSelectPortNotify(ClientPtr client)
{
    XvPortPtr pPort;

    REQUEST(xvSelectPortNotifyReq);
    REQUEST_SIZE_MATCH(xvSelectPortNotifyReq);

    VALIDATE_XV_PORT(stuff->port, pPort, DixReadAccess);

    return XvdiSelectPortNotify(client, pPort, stuff->onoff);
}
