static void ConvertDoubleToURational(double value, unsigned int *numerator,
 unsigned int *denominator) {
    float2urat(value, 0xFFFFFFFFU, numerator, denominator);
}
