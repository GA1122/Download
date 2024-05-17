xmlXPtrLocationSetRemove(xmlLocationSetPtr cur, int val) {
    if (cur == NULL) return;
    if (val >= cur->locNr) return;
    cur->locNr--;
    for (;val < cur->locNr;val++)
        cur->locTab[val] = cur->locTab[val + 1];
    cur->locTab[cur->locNr] = NULL;
}
