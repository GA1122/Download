void PluginServiceImpl::OpenChannelToNpapiPlugin(
    int render_process_id,
    int render_view_id,
    const GURL& url,
    const GURL& page_url,
    const std::string& mime_type,
    PluginProcessHost::Client* client) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));
  DCHECK(!ContainsKey(pending_plugin_clients_, client));
  pending_plugin_clients_.insert(client);

  PluginServiceFilterParams params = {
    render_process_id,
    render_view_id,
    page_url,
    client->GetResourceContext()
  };
  GetPlugins(base::Bind(
      &PluginServiceImpl::ForwardGetAllowedPluginForOpenChannelToPlugin,
      base::Unretained(this), params, url, mime_type, client));
}
