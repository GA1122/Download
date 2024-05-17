PassRefPtr<MainResourceLoader> MainResourceLoader::create(Frame* frame)
{
    return adoptRef(new MainResourceLoader(frame));
}
