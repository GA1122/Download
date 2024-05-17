static void esp_soft_reset(ESPState *s)
{
    qemu_irq_lower(s->irq);
    esp_hard_reset(s);
}
