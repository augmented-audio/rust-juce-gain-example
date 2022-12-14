use audio_processor_traits::audio_buffer::vst::VSTBufferHandler;
use audio_processor_traits::{AudioBuffer, AudioProcessor};
pub use juce::*;

mod juce;

pub struct GainProcessor {
    gain: f32,
}

impl AudioProcessor for GainProcessor {
    type SampleType = f32;

    fn process<BufferType: AudioBuffer<SampleType=Self::SampleType>>(&mut self, data: &mut BufferType) {
        for frame in data.frames_mut() {
            for channel in frame {
                *channel *= self.gain;
            }
        }
    }
}

pub struct JUCEAudioProcessorContext {
    processor: GainProcessor,
    handler: VSTBufferHandler<f32>
}

#[no_mangle]
pub extern "C" fn audio_processor_context__init() -> *mut JUCEAudioProcessorContext {
    wisual_logger::init_from_env();
    log::info!("GainPlugin - Initializing");

    let processor = GainProcessor {
        gain: 1.0,
    };
    let ctx = JUCEAudioProcessorContext {
        processor,
        handler: VSTBufferHandler::new(),
    };
    Box::into_raw(Box::new(ctx))
}

#[no_mangle]
pub extern "C" fn audio_processor_context__drop(context: *mut JUCEAudioProcessorContext) {
    let _ = unsafe { Box::from_raw(context) };
}

#[no_mangle]
pub extern "C" fn audio_processor_context__process_buffer(context: &mut JUCEAudioProcessorContext, parameters: JUCEParameters, buffer: *mut JUCESimpleAudioBuffer) {
    let mut buffer = unsafe {
        let buffer = &*buffer;
        vst::buffer::AudioBuffer::from_raw(
            buffer.num_channels,
            buffer.num_channels,
            buffer.channels.as_ptr() as *const *const f32,
            buffer.channels.as_ptr() as *mut *mut f32,
            buffer.num_samples
        )
    };

    let processor = &mut context.processor;
    processor.gain = parameters.gain;
    let handler = &mut context.handler;
    handler.with_buffer(&mut buffer, |buffer| {
        processor.process(buffer);
    });
}