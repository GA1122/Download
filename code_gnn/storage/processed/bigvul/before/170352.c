static void convertTimeToDate(int64_t time_1904, String8 *s) {
 time_t time_1970 = time_1904 - (((66 * 365 + 17) * 24) * 3600);

 char tmp[32];
    strftime(tmp, sizeof(tmp), "%Y%m%dT%H%M%S.000Z", gmtime(&time_1970));

    s->setTo(tmp);
}
