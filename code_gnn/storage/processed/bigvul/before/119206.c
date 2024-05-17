void XMLHttpRequest::send(ArrayBufferView* body, ExceptionState& es)
{
    HistogramSupport::histogramEnumeration("WebCore.XHR.send.ArrayBufferOrView", XMLHttpRequestSendArrayBufferView, XMLHttpRequestSendArrayBufferOrViewMax);

    sendBytesData(body->baseAddress(), body->byteLength(), es);
}
