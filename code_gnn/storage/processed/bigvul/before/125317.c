void Player::finish(ExceptionState& exceptionState)
{
    if (!m_playbackRate)
        return;
    if (m_playbackRate < 0) {
        setCurrentTime(0);
    } else {
        if (sourceEnd() == std::numeric_limits<double>::infinity()) {
            exceptionState.throwDOMException(InvalidStateError, "Player has source content whose end time is infinity.");
            return;
        }
        setCurrentTime(sourceEnd());
    }
    ASSERT(finished());
}
