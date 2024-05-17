Frame* InspectorPageAgent::assertFrame(ErrorString* errorString, const String& frameId)
{
    Frame* frame = frameForId(frameId);
    if (!frame)
        *errorString = "No frame for given id found";
    return frame;
}
