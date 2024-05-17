void parseVorbisComment(
 const sp<MetaData> &fileMeta, const char *comment, size_t commentLength)
{
 struct {
 const char *const mTag;
 uint32_t mKey;
 } kMap[] = {
 { "TITLE", kKeyTitle },
 { "ARTIST", kKeyArtist },
 { "ALBUMARTIST", kKeyAlbumArtist },
 { "ALBUM ARTIST", kKeyAlbumArtist },
 { "COMPILATION", kKeyCompilation },
 { "ALBUM", kKeyAlbum },
 { "COMPOSER", kKeyComposer },
 { "GENRE", kKeyGenre },
 { "AUTHOR", kKeyAuthor },
 { "TRACKNUMBER", kKeyCDTrackNumber },
 { "DISCNUMBER", kKeyDiscNumber },
 { "DATE", kKeyDate },
 { "YEAR", kKeyYear },
 { "LYRICIST", kKeyWriter },
 { "METADATA_BLOCK_PICTURE", kKeyAlbumArt },
 { "ANDROID_LOOP", kKeyAutoLoop },
 };

 for (size_t j = 0; j < sizeof(kMap) / sizeof(kMap[0]); ++j) {
 size_t tagLen = strlen(kMap[j].mTag);
 if (!strncasecmp(kMap[j].mTag, comment, tagLen)
 && comment[tagLen] == '=') {
 if (kMap[j].mKey == kKeyAlbumArt) {
                    extractAlbumArt(
                            fileMeta,
 &comment[tagLen + 1],
                            commentLength - tagLen - 1);
 } else if (kMap[j].mKey == kKeyAutoLoop) {
 if (!strcasecmp(&comment[tagLen + 1], "true")) {
                        fileMeta->setInt32(kKeyAutoLoop, true);
 }
 } else {
                    fileMeta->setCString(kMap[j].mKey, &comment[tagLen + 1]);
 }
 }
 }

}
