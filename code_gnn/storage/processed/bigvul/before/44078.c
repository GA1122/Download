static int schema_filter(const struct dirent * a)
{
    int rc = 0;
    float version = 0;

    if(strstr(a->d_name, "pacemaker-") != a->d_name) {
         

    } else if(strstr(a->d_name, ".rng") == NULL) {
         

    } else if(sscanf(a->d_name, "pacemaker-%f.rng", &version) == 0) {
         

    } else if(strcmp(a->d_name, "pacemaker-1.1.rng") == 0) {
         

    } else {
         
        rc = 1;
    }

    return rc;
}
