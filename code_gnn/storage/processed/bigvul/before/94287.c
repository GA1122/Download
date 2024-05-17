static enum test_return test_vperror(void) {
    int rv = 0;
    int oldstderr = dup(STDERR_FILENO);
    char tmpl[sizeof(TMP_TEMPLATE)+1];
    strncpy(tmpl, TMP_TEMPLATE, sizeof(TMP_TEMPLATE)+1);

    int newfile = mkstemp(tmpl);
    assert(newfile > 0);
    rv = dup2(newfile, STDERR_FILENO);
    assert(rv == STDERR_FILENO);
    rv = close(newfile);
    assert(rv == 0);

    errno = EIO;
    vperror("Old McDonald had a farm.  %s", "EI EIO");

     
    rv = dup2(oldstderr, STDERR_FILENO);
    assert(rv == STDERR_FILENO);


     
    char buf[80] = { 0 };
    FILE *efile = fopen(tmpl, "r");
    assert(efile);
    char *prv = fgets(buf, sizeof(buf), efile);
    assert(prv);
    fclose(efile);

    unlink(tmpl);

    char expected[80] = { 0 };
    snprintf(expected, sizeof(expected),
             "Old McDonald had a farm.  EI EIO: %s\n", strerror(EIO));

     

    return strcmp(expected, buf) == 0 ? TEST_PASS : TEST_FAIL;
}