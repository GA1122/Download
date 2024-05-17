void Frame::didChangeVisibilityState()
{
    HeapVector<Member<Frame>> childFrames;
    for (Frame* child = tree().firstChild(); child; child = child->tree().nextSibling())
        childFrames.append(child);
    for (size_t i = 0; i < childFrames.size(); ++i)
         childFrames[i]->didChangeVisibilityState();
 }
