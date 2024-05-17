static void enableCrossSiteXHRRecursively(Frame* frame)
{
    frame->document()->securityOrigin()->grantUniversalAccess();

    Vector<RefPtr<Frame>, 10> childFrames;
    for (RefPtr<Frame> childFrame = frame->tree()->firstChild(); childFrame; childFrame = childFrame->tree()->nextSibling())
        childFrames.append(childFrame);

    unsigned size = childFrames.size();
    for (unsigned i = 0; i < size; i++)
        enableCrossSiteXHRRecursively(childFrames[i].get());
}
