void InspectorResourceAgent::didFinishLoading(unsigned long identifier, DocumentLoader* loader, double monotonicFinishTime, int64_t encodedDataLength)
{
    double finishTime = 0.0;
    if (monotonicFinishTime)
        finishTime = loader->timing()->monotonicTimeToPseudoWallTime(monotonicFinishTime);

    String requestId = IdentifiersFactory::requestId(identifier);
    m_resourcesData->maybeDecodeDataToContent(requestId);
    if (!finishTime)
        finishTime = currentTime();
    m_frontend->loadingFinished(requestId, finishTime, encodedDataLength);
}
