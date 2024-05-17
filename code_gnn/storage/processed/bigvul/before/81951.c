static sds cliVersion(void) {
    sds version;
    version = sdscatprintf(sdsempty(), "%s", REDIS_VERSION);

     
    if (strtoll(redisGitSHA1(),NULL,16)) {
        version = sdscatprintf(version, " (git:%s", redisGitSHA1());
        if (strtoll(redisGitDirty(),NULL,10))
            version = sdscatprintf(version, "-dirty");
        version = sdscat(version, ")");
    }
    return version;
}