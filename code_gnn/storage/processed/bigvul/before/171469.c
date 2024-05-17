bool ASessionDescription::parse(const void *data, size_t size) {
    mTracks.clear();
    mFormats.clear();

    mTracks.push(Attribs());
    mFormats.push(AString("[root]"));

 AString desc((const char *)data, size);

 size_t i = 0;
 for (;;) {
 ssize_t eolPos = desc.find("\n", i);

 if (eolPos < 0) {
 break;
 }

 AString line;
 if ((size_t)eolPos > i && desc.c_str()[eolPos - 1] == '\r') {
            line.setTo(desc, i, eolPos - i - 1);
 } else {
            line.setTo(desc, i, eolPos - i);
 }

 if (line.empty()) {
            i = eolPos + 1;
 continue;
 }

 if (line.size() < 2 || line.c_str()[1] != '=') {
 return false;
 }

        ALOGI("%s", line.c_str());

 switch (line.c_str()[0]) {
 case 'v':
 {
 if (strcmp(line.c_str(), "v=0")) {
 return false;
 }
 break;
 }

 case 'a':
 case 'b':
 {
 AString key, value;

 ssize_t colonPos = line.find(":", 2);
 if (colonPos < 0) {
                    key = line;
 } else {
                    key.setTo(line, 0, colonPos);

 if (key == "a=fmtp" || key == "a=rtpmap"
 || key == "a=framesize") {
 ssize_t spacePos = line.find(" ", colonPos + 1);
 if (spacePos < 0) {
 return false;
 }

                        key.setTo(line, 0, spacePos);

                        colonPos = spacePos;
 }

                    value.setTo(line, colonPos + 1, line.size() - colonPos - 1);
 }

                key.trim();
                value.trim();

                ALOGV("adding '%s' => '%s'", key.c_str(), value.c_str());

                mTracks.editItemAt(mTracks.size() - 1).add(key, value);
 break;
 }

 case 'm':
 {
                ALOGV("new section '%s'",
 AString(line, 2, line.size() - 2).c_str());

                mTracks.push(Attribs());
                mFormats.push(AString(line, 2, line.size() - 2));
 break;
 }

 default:
 {
 AString key, value;

 ssize_t equalPos = line.find("=");

                key = AString(line, 0, equalPos + 1);
                value = AString(line, equalPos + 1, line.size() - equalPos - 1);

                key.trim();
                value.trim();

                ALOGV("adding '%s' => '%s'", key.c_str(), value.c_str());

                mTracks.editItemAt(mTracks.size() - 1).add(key, value);
 break;
 }
 }

        i = eolPos + 1;
 }

 return true;
}
