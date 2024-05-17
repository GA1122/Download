MPEG4Extractor::Track *MPEG4Extractor::findTrackByMimePrefix(
 const char *mimePrefix) {
 for (Track *track = mFirstTrack; track != NULL; track = track->next) {
 const char *mime;
 if (track->meta != NULL
 && track->meta->findCString(kKeyMIMEType, &mime)
 && !strncasecmp(mime, mimePrefix, strlen(mimePrefix))) {
 return track;
 }
 }

 return NULL;
}
