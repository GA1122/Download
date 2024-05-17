void ResourceFetcher::LogPreloadStats(ClearPreloadsPolicy policy) {
  if (!preloads_)
    return;
  unsigned scripts = 0;
  unsigned script_misses = 0;
  unsigned stylesheets = 0;
  unsigned stylesheet_misses = 0;
  unsigned images = 0;
  unsigned image_misses = 0;
  unsigned fonts = 0;
  unsigned font_misses = 0;
  unsigned medias = 0;
  unsigned media_misses = 0;
  unsigned text_tracks = 0;
  unsigned text_track_misses = 0;
  unsigned imports = 0;
  unsigned import_misses = 0;
  unsigned raws = 0;
  unsigned raw_misses = 0;
  for (const auto& resource : *preloads_) {
    if (resource->IsLinkPreload() &&
        policy == kClearSpeculativeMarkupPreloads) {
      continue;
    }
    int miss_count =
        resource->GetPreloadResult() == Resource::kPreloadNotReferenced ? 1 : 0;
    switch (resource->GetType()) {
      case Resource::kImage:
        images++;
        image_misses += miss_count;
        break;
      case Resource::kScript:
        scripts++;
        script_misses += miss_count;
        break;
      case Resource::kCSSStyleSheet:
        stylesheets++;
        stylesheet_misses += miss_count;
        break;
      case Resource::kFont:
        fonts++;
        font_misses += miss_count;
        break;
      case Resource::kMedia:
        medias++;
        media_misses += miss_count;
        break;
      case Resource::kTextTrack:
        text_tracks++;
        text_track_misses += miss_count;
        break;
      case Resource::kImportResource:
        imports++;
        import_misses += miss_count;
        break;
      case Resource::kRaw:
        raws++;
        raw_misses += miss_count;
        break;
      case Resource::kMock:
        break;
      default:
        NOTREACHED();
    }
  }
  DEFINE_STATIC_LOCAL(CustomCountHistogram, image_preloads,
                      ("PreloadScanner.Counts2.Image", 0, 100, 25));
  DEFINE_STATIC_LOCAL(CustomCountHistogram, image_preload_misses,
                      ("PreloadScanner.Counts2.Miss.Image", 0, 100, 25));
  DEFINE_STATIC_LOCAL(CustomCountHistogram, script_preloads,
                      ("PreloadScanner.Counts2.Script", 0, 100, 25));
  DEFINE_STATIC_LOCAL(CustomCountHistogram, script_preload_misses,
                      ("PreloadScanner.Counts2.Miss.Script", 0, 100, 25));
  DEFINE_STATIC_LOCAL(CustomCountHistogram, stylesheet_preloads,
                      ("PreloadScanner.Counts2.CSSStyleSheet", 0, 100, 25));
  DEFINE_STATIC_LOCAL(
      CustomCountHistogram, stylesheet_preload_misses,
      ("PreloadScanner.Counts2.Miss.CSSStyleSheet", 0, 100, 25));
  DEFINE_STATIC_LOCAL(CustomCountHistogram, font_preloads,
                      ("PreloadScanner.Counts2.Font", 0, 100, 25));
  DEFINE_STATIC_LOCAL(CustomCountHistogram, font_preload_misses,
                      ("PreloadScanner.Counts2.Miss.Font", 0, 100, 25));
  DEFINE_STATIC_LOCAL(CustomCountHistogram, media_preloads,
                      ("PreloadScanner.Counts2.Media", 0, 100, 25));
  DEFINE_STATIC_LOCAL(CustomCountHistogram, media_preload_misses,
                      ("PreloadScanner.Counts2.Miss.Media", 0, 100, 25));
  DEFINE_STATIC_LOCAL(CustomCountHistogram, text_track_preloads,
                      ("PreloadScanner.Counts2.TextTrack", 0, 100, 25));
  DEFINE_STATIC_LOCAL(CustomCountHistogram, text_track_preload_misses,
                      ("PreloadScanner.Counts2.Miss.TextTrack", 0, 100, 25));
  DEFINE_STATIC_LOCAL(CustomCountHistogram, import_preloads,
                      ("PreloadScanner.Counts2.Import", 0, 100, 25));
  DEFINE_STATIC_LOCAL(CustomCountHistogram, import_preload_misses,
                      ("PreloadScanner.Counts2.Miss.Import", 0, 100, 25));
  DEFINE_STATIC_LOCAL(CustomCountHistogram, raw_preloads,
                      ("PreloadScanner.Counts2.Raw", 0, 100, 25));
  DEFINE_STATIC_LOCAL(CustomCountHistogram, raw_preload_misses,
                      ("PreloadScanner.Counts2.Miss.Raw", 0, 100, 25));
  if (images)
    image_preloads.Count(images);
  if (image_misses)
    image_preload_misses.Count(image_misses);
  if (scripts)
    script_preloads.Count(scripts);
  if (script_misses)
    script_preload_misses.Count(script_misses);
  if (stylesheets)
    stylesheet_preloads.Count(stylesheets);
  if (stylesheet_misses)
    stylesheet_preload_misses.Count(stylesheet_misses);
  if (fonts)
    font_preloads.Count(fonts);
  if (font_misses)
    font_preload_misses.Count(font_misses);
  if (medias)
    media_preloads.Count(medias);
  if (media_misses)
    media_preload_misses.Count(media_misses);
  if (text_tracks)
    text_track_preloads.Count(text_tracks);
  if (text_track_misses)
    text_track_preload_misses.Count(text_track_misses);
  if (imports)
    import_preloads.Count(imports);
  if (import_misses)
    import_preload_misses.Count(import_misses);
  if (raws)
    raw_preloads.Count(raws);
  if (raw_misses)
    raw_preload_misses.Count(raw_misses);
}
