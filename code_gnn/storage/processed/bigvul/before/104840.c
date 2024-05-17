  TestPrerenderContents(
      PrerenderManager* prerender_manager,
      Profile* profile,
      const GURL& url,
      const GURL& referrer,
      int number_of_loads,
      FinalStatus expected_final_status)
      : PrerenderContents(prerender_manager, profile, url, referrer),
        number_of_loads_(0),
        expected_number_of_loads_(number_of_loads),
        expected_final_status_(expected_final_status) {
  }
