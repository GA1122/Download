void WebPage::onCertificateStoreLocationSet(const BlackBerry::Platform::String& caPath)
{
#if ENABLE(VIDEO)
    MediaPlayerPrivate::setCertificatePath(caPath);
#endif
}
