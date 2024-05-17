variations::VariationsSeed CreateTestSeed() {
  variations::VariationsSeed seed;
  variations::Study* study = seed.add_study();
  study->set_name("test");
  study->set_default_experiment_name("abc");
  variations::Study_Experiment* experiment = study->add_experiment();
  experiment->set_name("abc");
  experiment->set_probability_weight(100);
  seed.set_serial_number("123");
  return seed;
}
