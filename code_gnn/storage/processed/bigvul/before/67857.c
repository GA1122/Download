IPV6DefragSturgesNovakLinuxTest(void)
{
     
    u_char expected[] = {
        "AAAAAAAA"
        "AAAAAAAA"
        "AAAAAAAA"
        "JJJJJJJJ"
        "JJJJJJJJ"
        "BBBBBBBB"
        "KKKKKKKK"
        "KKKKKKKK"
        "KKKKKKKK"
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
        "PPPPPPPP"
        "HHHHHHHH"
        "QQQQQQQQ"
        "QQQQQQQQ"
    };

    return IPV6DefragDoSturgesNovakTest(DEFRAG_POLICY_LINUX, expected,
        sizeof(expected));
}