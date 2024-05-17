main(int argc, char **argv)
{
    Jbig2Ctx *ctx;
    Jbig2HuffmanTable *tables[5];
    Jbig2HuffmanState *hs;
    Jbig2WordStream ws;
    bool oob;
    int32_t code;

    ctx = jbig2_ctx_new(NULL, 0, NULL, NULL, NULL);

    tables[0] = NULL;
    tables[1] = jbig2_build_huffman_table(ctx, &jbig2_huffman_params_A);
    tables[2] = jbig2_build_huffman_table(ctx, &jbig2_huffman_params_B);
    tables[3] = NULL;
    tables[4] = jbig2_build_huffman_table(ctx, &jbig2_huffman_params_D);
    ws.get_next_word = test_get_word;
    hs = jbig2_huffman_new(ctx, &ws);

    printf("testing jbig2 huffmann decoding...");
    printf("\t(should be 8 5 (oob) 8)\n");

    {
        int i;
        int sequence_length = sizeof(test_tabindex);

        for (i = 0; i < sequence_length; i++) {
            code = jbig2_huffman_get(hs, tables[test_tabindex[i]], &oob);
            if (oob)
                printf("(oob) ");
            else
                printf("%d ", code);
        }
    }

    printf("\n");

    jbig2_ctx_free(ctx);

    return 0;
}
