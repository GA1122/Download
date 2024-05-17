MostVisitedSitesBridge::MostVisitedSitesBridge(Profile* profile)
    : most_visited_(ChromeMostVisitedSitesFactory::NewForProfile(profile)) {
  content::URLDataSource::Add(profile, new ThumbnailListSource(profile));
  DCHECK(!profile->IsOffTheRecord());
}
