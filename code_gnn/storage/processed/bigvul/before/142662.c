bool FrameLoader::allAncestorsAreComplete() const
{
    for (Frame* ancestor = m_frame; ancestor; ancestor = ancestor->tree().parent()) {
        if (ancestor->isLoading())
            return false;
    }
    return true;
}
