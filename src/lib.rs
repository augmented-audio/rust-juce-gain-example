pub struct JUCESimpleAudioBuffer {
    #[allow(unused)]
    num_channels: usize,
    #[allow(unused)]
    num_samples: usize,
    #[allow(unused)]
    channels: Vec<*mut f32>,
}

#[repr(C)]
pub struct JUCEParameters {
    gain: f32,
}

#[no_mangle]
pub extern "C" fn compatibility_audio_buffer__new(num_channels: usize, num_samples: usize) -> *mut JUCESimpleAudioBuffer {
    let buffer = JUCESimpleAudioBuffer {
        num_channels,
        num_samples,
        channels: {
            let mut v = Vec::with_capacity(num_channels);
            v.resize(num_channels, 0 as *mut f32);
            v
        },
    };
    Box::into_raw(Box::new(buffer))
}

#[no_mangle]
pub extern "C" fn compatibility_audio_buffer__set_write_pointer(buffer: *mut JUCESimpleAudioBuffer, channel: usize, write_pointer: *mut f32) {
    unsafe {
        let buffer = &mut (*buffer);
        if channel >= buffer.channels.len() {
            buffer.channels.resize(channel + 1, std::ptr::null_mut());
        }
        buffer.channels[channel] = write_pointer;
    }
}

#[no_mangle]
pub extern "C" fn compatibility_audio_buffer__drop(buffer: *mut JUCESimpleAudioBuffer) {
    let _ = unsafe { Box::from_raw(buffer) };
}

#[no_mangle]
pub extern "C" fn gain__process_buffer(parameters: JUCEParameters, buffer: *mut JUCESimpleAudioBuffer) {
    let buffer = unsafe { &*buffer };
    for channel_num in 0..buffer.num_channels {
        let slice = unsafe { std::slice::from_raw_parts_mut(buffer.channels[channel_num], buffer.num_samples) };
        for sample in slice {
            *sample = *sample * parameters.gain;
        }
    }
}