void ReleaseAllMediaPlayers(content::WebContents* web_contents,
                            content::RenderFrameHost* render_frame_host) {
  content::BrowserMediaPlayerManager* manager =
      static_cast<content::WebContentsImpl*>(web_contents)->
          media_web_contents_observer()->GetMediaPlayerManager(
              render_frame_host);
  if (manager)
    manager->ReleaseAllMediaPlayers();
}
