#include <cstdarg>
#include <cstdint>
#include <cstdlib>
#include <ostream>
#include <new>

struct JUCEAudioProcessorContext;

struct JUCESimpleAudioBuffer;

struct JUCEParameters {
  float gain;
};

extern "C" {

JUCEAudioProcessorContext *audio_processor_context__init();

void audio_processor_context__drop(JUCEAudioProcessorContext *context);

void audio_processor_context__process_buffer(JUCEAudioProcessorContext *context,
                                             JUCEParameters parameters,
                                             JUCESimpleAudioBuffer *buffer);

JUCESimpleAudioBuffer *compatibility_audio_buffer__new(uintptr_t num_channels,
                                                       uintptr_t num_samples);

void compatibility_audio_buffer__set_write_pointer(JUCESimpleAudioBuffer *buffer,
                                                   uintptr_t channel,
                                                   float *write_pointer);

void compatibility_audio_buffer__drop(JUCESimpleAudioBuffer *buffer);

} // extern "C"
