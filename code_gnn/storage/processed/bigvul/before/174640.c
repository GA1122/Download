void sysReleaseMap(MemMapping* pMap)
{
 int i;
 for (i = 0; i < pMap->range_count; ++i) {
 if (munmap(pMap->ranges[i].addr, pMap->ranges[i].length) < 0) {
            LOGW("munmap(%p, %d) failed: %s\n",
                 pMap->ranges[i].addr, (int)pMap->ranges[i].length, strerror(errno));
 }
 }
    free(pMap->ranges);
    pMap->ranges = NULL;
    pMap->range_count = 0;
}
