void XMLHttpRequest::send(ArrayBuffer* body, ExceptionState& es)
{
    String consoleMessage("ArrayBuffer is deprecated in XMLHttpRequest.send(). Use ArrayBufferView instead.");
    scriptExecutionContext()->addConsoleMessage(JSMessageSource, WarningMessageLevel, consoleMessage);

    HistogramSupport::histogramEnumeration("WebCore.XHR.send.ArrayBufferOrView", XMLHttpRequestSendArrayBuffer, XMLHttpRequestSendArrayBufferOrViewMax);

    sendBytesData(body->data(), body->byteLength(), es);
}
