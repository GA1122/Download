ChromeContentRendererClient::OverrideCreateWebMediaPlayer(
    content::RenderView* render_view,
    WebKit::WebFrame* frame,
    WebKit::WebMediaPlayerClient* client,
    base::WeakPtr<webkit_media::WebMediaPlayerDelegate> delegate,
    media::FilterCollection* collection,
    WebKit::WebAudioSourceProvider* audio_source_provider,
    media::MessageLoopFactory* message_loop_factory,
    webkit_media::MediaStreamClient* media_stream_client,
    media::MediaLog* media_log) {
  if (!prerender::PrerenderHelper::IsPrerendering(render_view))
    return NULL;

  return new prerender::PrerenderWebMediaPlayer(render_view, frame, client,
      delegate, collection, audio_source_provider, message_loop_factory,
      media_stream_client, media_log);
}
