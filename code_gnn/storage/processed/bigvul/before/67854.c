IPV6DefragSturgesNovakBsdTest(void)
{
     
    u_char expected[] = {
        "AAAAAAAA"
        "AAAAAAAA"
        "AAAAAAAA"
        "JJJJJJJJ"
        "JJJJJJJJ"
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

    return IPV6DefragDoSturgesNovakTest(DEFRAG_POLICY_BSD, expected, sizeof(expected));
}