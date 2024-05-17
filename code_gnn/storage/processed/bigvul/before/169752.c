_exsltDateCastYMToDays (const exsltDateValPtr dt)
{
    long ret;

    if (dt->value.date.year < 0)
        ret = (dt->value.date.year * 365) +
              (((dt->value.date.year+1)/4)-((dt->value.date.year+1)/100)+
               ((dt->value.date.year+1)/400)) +
              DAY_IN_YEAR(0, dt->value.date.mon, dt->value.date.year);
    else
        ret = ((dt->value.date.year-1) * 365) +
              (((dt->value.date.year-1)/4)-((dt->value.date.year-1)/100)+
               ((dt->value.date.year-1)/400)) +
              DAY_IN_YEAR(0, dt->value.date.mon, dt->value.date.year);

    return ret;
}
