#include <cstdarg>
#include <cstdint>
#include <cstdlib>
#include <ostream>
#include <new>

struct JUCESimpleAudioBuffer;

extern "C" {

JUCESimpleAudioBuffer *compatibility_audio_buffer__new(uintptr_t num_channels,
                                                       uintptr_t num_samples);

void compatibility_audio_buffer__set_write_pointer(JUCESimpleAudioBuffer *buffer,
                                                   uintptr_t channel,
                                                   float *write_pointer);

void compatibility_audio_buffer__drop(JUCESimpleAudioBuffer *buffer);

void gain__process_buffer(JUCESimpleAudioBuffer *_buffer);

float gain_process(float input);

} // extern "C"
