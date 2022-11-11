#include "PluginEditor.h"
#include "PluginProcessor.h"

GainPluginAudioProcessor::GainPluginAudioProcessor ()
    : AudioProcessor (
          BusesProperties ()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
              .withInput ("Input", juce::AudioChannelSet::stereo (), true)
#endif
              .withOutput ("Output", juce::AudioChannelSet::stereo (), true)
#endif
      )
{
    gain = new juce::AudioParameterFloat (
        juce::ParameterID ("gain", 1), "Gain", 0.0, 1.0, 0.0);
    addParameter (gain);
}

GainPluginAudioProcessor::~GainPluginAudioProcessor () = default;

const juce::String GainPluginAudioProcessor::getName () const
{
    return JucePlugin_Name;
}

bool GainPluginAudioProcessor::acceptsMidi () const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool GainPluginAudioProcessor::producesMidi () const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool GainPluginAudioProcessor::isMidiEffect () const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double GainPluginAudioProcessor::getTailLengthSeconds () const
{
    return 0.0;
}

int GainPluginAudioProcessor::getNumPrograms ()
{
    return 1; // NB: some hosts don't cope very well if you tell them there are
              // 0 programs,
    // so this should be at least 1, even if you're not really implementing
    // programs.
}

int GainPluginAudioProcessor::getCurrentProgram ()
{
    return 0;
}

void GainPluginAudioProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String GainPluginAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void GainPluginAudioProcessor::changeProgramName (int index,
                                                  const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

void GainPluginAudioProcessor::prepareToPlay (double sampleRate,
                                              int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    juce::ignoreUnused (sampleRate, samplesPerBlock);

    rustBuffer = compatibility_audio_buffer__new (
        static_cast<uintptr_t> (getTotalNumInputChannels ()),
        static_cast<uintptr_t> (samplesPerBlock));
}

void GainPluginAudioProcessor::releaseResources ()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    compatibility_audio_buffer__drop (rustBuffer);
    rustBuffer = nullptr;
}

bool GainPluginAudioProcessor::isBusesLayoutSupported (
    const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet () != juce::AudioChannelSet::mono () &&
        layouts.getMainOutputChannelSet () != juce::AudioChannelSet::stereo ())
        return false;

        // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet () != layouts.getMainInputChannelSet ())
        return false;
#endif

    return true;
#endif
}

void GainPluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                             juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels ();
    auto totalNumOutputChannels = getTotalNumOutputChannels ();

    int numSamples = buffer.getNumSamples ();

    for (auto channelNum = totalNumInputChannels;
         channelNum < totalNumOutputChannels;
         ++channelNum)
    {
        buffer.clear (channelNum, 0, numSamples);
    }

    for (auto channelNum = 0; channelNum < totalNumOutputChannels; channelNum++)
    {
        compatibility_audio_buffer__set_write_pointer (
            rustBuffer,
            static_cast<uintptr_t> (channelNum),
            buffer.getWritePointer (channelNum));

        JUCEParameters parameters = {
                gain->get(),
        };
        gain__process_buffer (parameters, rustBuffer);
    }
}

bool GainPluginAudioProcessor::hasEditor () const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* GainPluginAudioProcessor::createEditor ()
{
    return new juce::GenericAudioProcessorEditor (*this);
}

void GainPluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::ignoreUnused (destData);
}

void GainPluginAudioProcessor::setStateInformation (const void* data,
                                                    int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory
    // block, whose contents will have been created by the getStateInformation()
    // call.
    juce::ignoreUnused (data, sizeInBytes);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter ()
{
    return new GainPluginAudioProcessor ();
}
