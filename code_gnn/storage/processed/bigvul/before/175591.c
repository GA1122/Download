status_t NuMediaExtractor::getFileFormat(sp<AMessage> *format) const {
 Mutex::Autolock autoLock(mLock);

 *format = NULL;

 if (mImpl == NULL) {
 return -EINVAL;
 }

    sp<MetaData> meta = mImpl->getMetaData();

 const char *mime;
    CHECK(meta->findCString(kKeyMIMEType, &mime));
 *format = new AMessage();
 (*format)->setString("mime", mime);

 uint32_t type;
 const void *pssh;
 size_t psshsize;
 if (meta->findData(kKeyPssh, &type, &pssh, &psshsize)) {
        sp<ABuffer> buf = new ABuffer(psshsize);
        memcpy(buf->data(), pssh, psshsize);
 (*format)->setBuffer("pssh", buf);
 }

 return OK;
}
