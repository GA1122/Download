static int inttgt_to_output(int inttgt)
{
    int i;

    for (i = 0; i < ARRAY_SIZE(inttgt_output); i++) {
        if (inttgt_output[i][0] == inttgt) {
            return inttgt_output[i][1];
        }
    }

    fprintf(stderr, "%s: unsupported inttgt %d\n", __func__, inttgt);
    return OPENPIC_OUTPUT_INT;
}
