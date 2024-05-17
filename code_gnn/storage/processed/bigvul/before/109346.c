static PassRefPtr<TypeBuilder::Network::ResourceTiming> buildObjectForTiming(const ResourceLoadTiming& timing, DocumentLoader* loader)
{
    return TypeBuilder::Network::ResourceTiming::create()
        .setRequestTime(loader->timing()->monotonicTimeToPseudoWallTime(timing.requestTime))
        .setProxyStart(timing.calculateMillisecondDelta(timing.proxyStart))
        .setProxyEnd(timing.calculateMillisecondDelta(timing.proxyEnd))
        .setDnsStart(timing.calculateMillisecondDelta(timing.dnsStart))
        .setDnsEnd(timing.calculateMillisecondDelta(timing.dnsEnd))
        .setConnectStart(timing.calculateMillisecondDelta(timing.connectStart))
        .setConnectEnd(timing.calculateMillisecondDelta(timing.connectEnd))
        .setSslStart(timing.calculateMillisecondDelta(timing.sslStart))
        .setSslEnd(timing.calculateMillisecondDelta(timing.sslEnd))
        .setSendStart(timing.calculateMillisecondDelta(timing.sendStart))
        .setSendEnd(timing.calculateMillisecondDelta(timing.sendEnd))
        .setReceiveHeadersEnd(timing.calculateMillisecondDelta(timing.receiveHeadersEnd))
        .release();
}
