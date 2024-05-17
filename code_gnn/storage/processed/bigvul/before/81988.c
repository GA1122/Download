void showLatencyDistSamples(struct distsamples *samples, long long tot) {
    int j;

      
    printf("\033[38;5;0m");  
    for (j = 0; ; j++) {
        int coloridx =
            ceil((float) samples[j].count / tot * (spectrum_palette_size-1));
        int color = spectrum_palette[coloridx];
        printf("\033[48;5;%dm%c", (int)color, samples[j].character);
        samples[j].count = 0;
        if (samples[j].max == 0) break;  
    }
    printf("\033[0m\n");
    fflush(stdout);
}
