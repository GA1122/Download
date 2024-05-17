  bool HasOriginAttemptedToPreconnect(const GURL& origin) {
    DCHECK_EQ(origin, origin.GetOrigin());
    return base::Contains(preconnect_url_attempts_, origin);
  }
