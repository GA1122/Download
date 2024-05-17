ImageBitmapFactories::ImageBitmapLoader::ImageBitmapLoader(ImageBitmapFactories& factory, PassRefPtr<ScriptPromiseResolver> resolver, const IntRect& cropRect)
    : m_scriptState(ScriptState::current())
    , m_loader(FileReaderLoader::ReadAsArrayBuffer, this)
    , m_factory(&factory)
    , m_resolver(resolver)
    , m_cropRect(cropRect)
{
}