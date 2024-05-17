static bool allDescendantsAreComplete(Frame* frame)
{
    for (Frame* child = frame->tree().firstChild(); child; child = child->tree().traverseNext(frame)) {
        if (child->isLoading())
            return false;
    }
    return true;
}
