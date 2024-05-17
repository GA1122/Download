DefragInit(void)
{
    intmax_t tracker_pool_size;
    if (!ConfGetInt("defrag.trackers", &tracker_pool_size)) {
        tracker_pool_size = DEFAULT_DEFRAG_HASH_SIZE;
    }

     
    DefragPolicyLoadFromConfig();

     
    defrag_context = DefragContextNew();
    if (defrag_context == NULL) {
        SCLogError(SC_ERR_MEM_ALLOC,
            "Failed to allocate memory for the Defrag module.");
        exit(EXIT_FAILURE);
    }

    DefragSetDefaultTimeout(defrag_context->timeout);
    DefragInitConfig(FALSE);
}
