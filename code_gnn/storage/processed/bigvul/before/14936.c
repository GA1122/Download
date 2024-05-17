Dispatch(void)
{
    int *clientReady;            
    int result;
    ClientPtr client;
    int nready;
    HWEventQueuePtr *icheck = checkForInput;
    long start_tick;

    nextFreeClientID = 1;
    nClients = 0;

    clientReady = malloc(sizeof(int) * MaxClients);
    if (!clientReady)
        return;

    SmartScheduleSlice = SmartScheduleInterval;
    while (!dispatchException) {
        if (*icheck[0] != *icheck[1]) {
            ProcessInputEvents();
            FlushIfCriticalOutputPending();
        }

        nready = WaitForSomething(clientReady);

        if (nready && !SmartScheduleDisable) {
            clientReady[0] = SmartScheduleClient(clientReady, nready);
            nready = 1;
        }
        

        while (!dispatchException && (--nready >= 0)) {
            client = clients[clientReady[nready]];
            if (!client) {
                 
                continue;
            }
             
            if (grabState == GrabKickout) {
                grabState = GrabActive;
                break;
            }
            isItTimeToYield = FALSE;

            start_tick = SmartScheduleTime;
            while (!isItTimeToYield) {
                if (*icheck[0] != *icheck[1])
                    ProcessInputEvents();

                FlushIfCriticalOutputPending();
                if (!SmartScheduleDisable &&
                    (SmartScheduleTime - start_tick) >= SmartScheduleSlice) {
                     
                    if (client->smart_priority > SMART_MIN_PRIORITY)
                        client->smart_priority--;
                    break;
                }
                 

                 
                UpdateCurrentTimeIf();
                result = ReadRequestFromClient(client);
                if (result <= 0) {
                    if (result < 0)
                        CloseDownClient(client);
                    break;
                }

                client->sequence++;
                client->majorOp = ((xReq *) client->requestBuffer)->reqType;
                client->minorOp = 0;
                if (client->majorOp >= EXTENSION_BASE) {
                    ExtensionEntry *ext = GetExtensionEntry(client->majorOp);

                    if (ext)
                        client->minorOp = ext->MinorOpcode(client);
                }
#ifdef XSERVER_DTRACE
                if (XSERVER_REQUEST_START_ENABLED())
                    XSERVER_REQUEST_START(LookupMajorName(client->majorOp),
                                          client->majorOp,
                                          ((xReq *) client->requestBuffer)->length,
                                          client->index,
                                          client->requestBuffer);
#endif
                if (result > (maxBigRequestSize << 2))
                    result = BadLength;
                else {
                    result = XaceHookDispatch(client, client->majorOp);
                    if (result == Success)
                        result =
                            (*client->requestVector[client->majorOp]) (client);
                    XaceHookAuditEnd(client, result);
                }
#ifdef XSERVER_DTRACE
                if (XSERVER_REQUEST_DONE_ENABLED())
                    XSERVER_REQUEST_DONE(LookupMajorName(client->majorOp),
                                         client->majorOp, client->sequence,
                                         client->index, result);
#endif

                if (client->noClientException != Success) {
                    CloseDownClient(client);
                    break;
                }
                else if (result != Success) {
                    SendErrorToClient(client, client->majorOp,
                                      client->minorOp,
                                      client->errorValue, result);
                    break;
                }
            }
            FlushAllOutput();
            client = clients[clientReady[nready]];
            if (client)
                client->smart_stop_tick = SmartScheduleTime;
        }
        dispatchException &= ~DE_PRIORITYCHANGE;
    }
#if defined(DDXBEFORERESET)
    ddxBeforeReset();
#endif
    KillAllClients();
    free(clientReady);
    dispatchException &= ~DE_RESET;
    SmartScheduleLatencyLimited = 0;
    ResetOsBuffers();
}
