DefragSturgesNovakSolarisTest(void)
{
     
    u_char expected[] = {
        "AAAAAAAA"
        "AAAAAAAA"
        "AAAAAAAA"
        "JJJJJJJJ"
        "BBBBBBBB"
        "BBBBBBBB"
        "CCCCCCCC"
        "CCCCCCCC"
        "CCCCCCCC"
        "LLLLLLLL"
        "LLLLLLLL"
        "LLLLLLLL"
        "MMMMMMMM"
        "MMMMMMMM"
        "MMMMMMMM"
        "FFFFFFFF"
        "FFFFFFFF"
        "FFFFFFFF"
        "GGGGGGGG"
        "GGGGGGGG"
        "HHHHHHHH"
        "HHHHHHHH"
        "IIIIIIII"
        "QQQQQQQQ"
    };

    return DefragDoSturgesNovakTest(DEFRAG_POLICY_SOLARIS, expected, sizeof(expected));
}