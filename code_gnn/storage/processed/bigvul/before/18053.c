main(int argc, char **argv)
{
    Jbig2Ctx *ctx = jbig2_ctx_new(NULL, 0, NULL, NULL, NULL);
    int i;

    for (i = 0; i < countof(tests); i++) {
        Jbig2HuffmanTable *table;
        Jbig2HuffmanState *hs;
        test_stream_t st;
        int32_t code;
        bool oob;
        int j;

        st.ws.get_next_word = test_get_word;
        st.h = &tests[i];
        printf("testing Standard Huffman table %s: ", st.h->name);
        table = jbig2_build_huffman_table(ctx, st.h->params);
        if (table == NULL) {
            printf("jbig2_build_huffman_table() returned NULL!\n");
        } else {
             
            hs = jbig2_huffman_new(ctx, &st.ws);
            if (hs == NULL) {
                printf("jbig2_huffman_new() returned NULL!\n");
            } else {
                for (j = 0; j < st.h->output_len; j++) {
                    printf("%d...", st.h->output[j]);
                    code = jbig2_huffman_get(hs, table, &oob);
                    if (code == st.h->output[j] && !oob) {
                        printf("ok, ");
                    } else {
                        int need_comma = 0;

                        printf("NG(");
                        if (code != st.h->output[j]) {
                            printf("%d", code);
                            need_comma = 1;
                        }
                        if (oob) {
                            if (need_comma)
                                printf(",");
                            printf("OOB");
                        }
                        printf("), ");
                    }
                }
                if (st.h->params->HTOOB) {
                    printf("OOB...");
                    code = jbig2_huffman_get(hs, table, &oob);
                    if (oob) {
                        printf("ok");
                    } else {
                        printf("NG(%d)", code);
                    }
                }
                printf("\n");
                jbig2_huffman_free(ctx, hs);
            }
            jbig2_release_huffman_table(ctx, table);
        }
    }
    jbig2_ctx_free(ctx);
    return 0;
}
