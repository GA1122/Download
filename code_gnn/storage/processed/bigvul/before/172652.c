void MediaPlayer::clear_l()
{
    mCurrentPosition = -1;
    mSeekPosition = -1;
    mVideoWidth = mVideoHeight = 0;
    mRetransmitEndpointValid = false;
}
