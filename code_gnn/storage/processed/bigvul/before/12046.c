int CRYPTO_THREAD_run_once(CRYPTO_ONCE *once, void (*init)(void))
{
    if (*once != 0)
        return 1;

    init();
    *once = 1;

    return 1;
}
