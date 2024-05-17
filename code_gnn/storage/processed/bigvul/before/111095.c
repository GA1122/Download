static void closeURLRecursively(Frame* frame)
{
    FrameLoaderClientBlackBerry* frameLoaderClient = static_cast<FrameLoaderClientBlackBerry*>(frame->loader()->client());
    frameLoaderClient->suppressChildFrameCreation();

    frame->loader()->closeURL();

    Vector<RefPtr<Frame>, 10> childFrames;

    for (RefPtr<Frame> childFrame = frame->tree()->firstChild(); childFrame; childFrame = childFrame->tree()->nextSibling())
        childFrames.append(childFrame);

    unsigned size = childFrames.size();
    for (unsigned i = 0; i < size; i++)
        closeURLRecursively(childFrames[i].get());
}
