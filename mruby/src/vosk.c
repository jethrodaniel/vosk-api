#include <mruby.h>
#include <mruby/array.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include <mruby/string.h>

#include <stdlib.h>
#include <string.h>

#include <vosk_api.h>

static void mrb_vosk_model_free(mrb_state *mrb, void *ptr) {
  vosk_model_free((VoskModel *)ptr);
}

static void mrb_vosk_recognizer_free(mrb_state *mrb, void *ptr) {
  vosk_recognizer_free((VoskRecognizer *)ptr);
}

struct mrb_data_type mrb_vosk_model_type = {"Model", mrb_vosk_model_free};
struct mrb_data_type mrb_vosk_recognizer_type = {"Recognizer",
                                                 mrb_vosk_recognizer_free};

static mrb_value mrb_vosk_model_initialize(mrb_state *mrb, mrb_value self) {
  VoskModel *model;
  mrb_value model_path;

  mrb_get_args(mrb, "S", &model_path);

  model = vosk_model_new(mrb_str_to_cstr(mrb, model_path));

  DATA_PTR(self) = model;
  DATA_TYPE(self) = &mrb_vosk_model_type;

  return self;
}

static mrb_value mrb_vosk_recognizer_initialize(mrb_state *mrb,
                                                mrb_value self) {
  VoskRecognizer *recognizer;
  VoskModel *vosk_model;

  mrb_value model;
  mrb_float sample_rate;

  mrb_get_args(mrb, "of", &model, &sample_rate);

  vosk_model =
      (struct VoskModel *)mrb_data_get_ptr(mrb, model, &mrb_vosk_model_type);

  recognizer = vosk_recognizer_new(vosk_model, (float)sample_rate);

  DATA_PTR(self) = recognizer;
  DATA_TYPE(self) = &mrb_vosk_recognizer_type;

  return self;
}

static mrb_value mrb_vosk_recognizer_accept_waveform(mrb_state *mrb,
                                                     mrb_value self) {
  VoskRecognizer *recognizer = (struct VoskRecognizer *)mrb_data_get_ptr(
      mrb, self, &mrb_vosk_recognizer_type);

  mrb_value audio;
  mrb_int audio_size;

  mrb_get_args(mrb, "A", &audio);

  audio_size = mrb_integer(mrb_funcall(mrb, audio, "size", 0));

  int size = (int)audio_size;

  int buffer_size = sizeof(char) * size;
  char *buffer = (char *)malloc(buffer_size);

  for (int i = 0; i < size; i++) {
    mrb_value element = RARRAY_PTR(audio)[i];
    char value = (char)mrb_integer(element);
    buffer[i] = value;
  }

  int result = vosk_recognizer_accept_waveform(recognizer, buffer, buffer_size);

  free(buffer);

  return mrb_int_value(mrb, result);
}

static mrb_value mrb_vosk_recognizer_final_result(mrb_state *mrb,
                                                  mrb_value self) {
  VoskRecognizer *recognizer = (struct VoskRecognizer *)mrb_data_get_ptr(
      mrb, self, &mrb_vosk_recognizer_type);

  const char *result = vosk_recognizer_final_result(recognizer);

  return mrb_str_new(mrb, result, strlen(result));
}

static mrb_value mrb_vosk_set_log_level(mrb_state *mrb, mrb_value self) {
  mrb_int level;
  mrb_get_args(mrb, "i", &level);

  vosk_set_log_level((int)level);

  return mrb_nil_value();
}

void mrb_mruby_vosk_gem_init(mrb_state *mrb) {
  struct RClass *mrb_vosk, *mrb_vosk_model, *mrb_vosk_recognizer;

  mrb_vosk = mrb_define_module(mrb, "Vosk");

  mrb_define_module_function(mrb, mrb_vosk, "set_log_level",
                             mrb_vosk_set_log_level, MRB_ARGS_REQ(1));

  mrb_vosk_model =
      mrb_define_class_under(mrb, mrb_vosk, "Model", mrb->object_class);
  MRB_SET_INSTANCE_TT(mrb_vosk_model, MRB_TT_CDATA);
  mrb_define_method(mrb, mrb_vosk_model, "initialize",
                    mrb_vosk_model_initialize, MRB_ARGS_REQ(1));

  mrb_vosk_recognizer =
      mrb_define_class_under(mrb, mrb_vosk, "Recognizer", mrb->object_class);

  MRB_SET_INSTANCE_TT(mrb_vosk_recognizer, MRB_TT_CDATA);
  mrb_define_method(mrb, mrb_vosk_recognizer, "initialize",
                    mrb_vosk_recognizer_initialize, MRB_ARGS_REQ(2));
  mrb_define_method(mrb, mrb_vosk_recognizer, "accept_waveform",
                    mrb_vosk_recognizer_accept_waveform, MRB_ARGS_REQ(2));

  mrb_define_method(mrb, mrb_vosk_recognizer, "final_result",
                    mrb_vosk_recognizer_final_result, MRB_ARGS_NONE());
}

void mrb_mruby_vosk_gem_final(mrb_state *mrb) {
  // finalizer
}
