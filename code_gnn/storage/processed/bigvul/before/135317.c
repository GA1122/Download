String Document::outgoingReferrer() const
{
    if (securityOrigin()->isUnique()) {
        return String();
    }

    const Document* referrerDocument = this;
    if (LocalFrame* frame = m_frame) {
        while (frame->document()->isSrcdocDocument()) {
            frame = toLocalFrame(frame->tree().parent());
            ASSERT(frame);
        }
        referrerDocument = frame->document();
    }
    return referrerDocument->m_url.strippedForUseAsReferrer();
}
