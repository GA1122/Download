size_t MPEG4Extractor::countTracks() {
 status_t err;
 if ((err = readMetaData()) != OK) {
        ALOGV("MPEG4Extractor::countTracks: no tracks");
 return 0;
 }

 size_t n = 0;
 Track *track = mFirstTrack;
 while (track) {
 ++n;
        track = track->next;
 }

    ALOGV("MPEG4Extractor::countTracks: %zu tracks", n);
 return n;
}
