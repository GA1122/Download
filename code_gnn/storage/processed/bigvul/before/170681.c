static EAS_I16 ConvertSampleRate (EAS_U32 sampleRate)
{
 return (EAS_I16) (1200.0 * log10((double) sampleRate / (double) outputSampleRate) / log10(2.0));
}
