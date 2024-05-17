intro_point_should_expire_now(rend_intro_point_t *intro,
                              time_t now)
{
  tor_assert(intro != NULL);

  if (intro->time_published == -1) {
     
    return 0;
  }

  if (intro_point_accepted_intro_count(intro) >=
      intro->max_introductions) {
     
    return 1;
  }

  if (intro->time_to_expire == -1) {
     
    int intro_point_lifetime_seconds =
      crypto_rand_int_range(INTRO_POINT_LIFETIME_MIN_SECONDS,
                            INTRO_POINT_LIFETIME_MAX_SECONDS);

     
    intro->time_to_expire = now + intro_point_lifetime_seconds;

    return 0;
  }

   
  return (now >= intro->time_to_expire);
}
