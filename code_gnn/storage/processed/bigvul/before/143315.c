void Frame::detachChildren()
{
    typedef HeapVector<Member<Frame>> FrameVector;
    FrameVector childrenToDetach;
    childrenToDetach.reserveCapacity(tree().childCount());
    for (Frame* child = tree().firstChild(); child; child = child->tree().nextSibling())
        childrenToDetach.append(child);
    for (const auto& child : childrenToDetach)
        child->detach(FrameDetachType::Remove);
}
