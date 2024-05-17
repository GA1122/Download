static SiteInstanceImpl* CreateSiteInstance(
    content::RenderProcessHostFactory* factory, const GURL& url) {
  SiteInstanceImpl* instance =
      reinterpret_cast<SiteInstanceImpl*>(
          SiteInstance::CreateForURL(NULL, url));
  instance->set_render_process_host_factory(factory);
  return instance;
}
