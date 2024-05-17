int getWxCompositingOperation(CompositeOperator op, bool hasAlpha)
{
    if (op == CompositeSourceOver && !hasAlpha)
        op = CompositeCopy;

    int function;
    switch (op) {
        case CompositeClear:
            function = wxCLEAR;
        case CompositeCopy:
            function = wxCOPY; 
            break;
        default:
            function = wxCOPY;
    }
    return function;
}
