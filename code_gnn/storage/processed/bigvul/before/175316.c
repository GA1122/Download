static bool convertTimeToDate(int64_t time_1904, String8 *s) {
 static const int64_t delta = (((66 * 365 + 17) * 24) * 3600);
 if (time_1904 < INT64_MIN + delta) {
 return false;
 }
 time_t time_1970 = time_1904 - delta;

 char tmp[32];
 struct tm* tm = gmtime(&time_1970);
 if (tm != NULL &&
            strftime(tmp, sizeof(tmp), "%Y%m%dT%H%M%S.000Z", tm) > 0) {
        s->setTo(tmp);
 return true;
 }
 return false;
}
