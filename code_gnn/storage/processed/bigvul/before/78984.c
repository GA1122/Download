ExprCreateFloat(void)
{
    EXPR_CREATE(ExprFloat, expr, EXPR_VALUE, EXPR_TYPE_FLOAT);
    return expr;
}