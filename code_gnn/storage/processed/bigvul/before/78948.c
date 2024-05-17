ExprResolveLevel(struct xkb_context *ctx, const ExprDef *expr,
                 xkb_level_index_t *level_rtrn)
{
    bool ok;
    int result;

    ok = ExprResolveIntegerLookup(ctx, expr, &result, SimpleLookup,
                                  levelNames);
    if (!ok)
        return false;

    if (result < 1) {
        log_err(ctx, "Shift level %d is out of range\n", result);
        return false;
    }

     
    *level_rtrn = (unsigned int) (result - 1);
    return true;
}
