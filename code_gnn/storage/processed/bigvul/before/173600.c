void NuPlayer::GenericSource::queueDiscontinuityIfNeeded(
 bool seeking, bool formatChange, media_track_type trackType, Track *track) {
 if ((seeking || formatChange)
 && (trackType == MEDIA_TRACK_TYPE_AUDIO
 || trackType == MEDIA_TRACK_TYPE_VIDEO)) {
 ATSParser::DiscontinuityType type = (formatChange && seeking)
 ? ATSParser::DISCONTINUITY_FORMATCHANGE
 : ATSParser::DISCONTINUITY_NONE;
        track->mPackets->queueDiscontinuity(type, NULL  , true  );
 }
}
