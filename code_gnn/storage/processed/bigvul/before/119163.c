PassRefPtr<XMLHttpRequest> XMLHttpRequest::create(ScriptExecutionContext* context, PassRefPtr<SecurityOrigin> securityOrigin)
{
    RefPtr<XMLHttpRequest> xmlHttpRequest(adoptRef(new XMLHttpRequest(context, securityOrigin)));
    xmlHttpRequest->suspendIfNeeded();

    return xmlHttpRequest.release();
}
