#include "Apsa23.hpp"
#include <stdlib.h>
#include <string>


AudioEffect* createEffectInstance(audioMasterCallback audioMaster){
    return new Apsa23(audioMaster);
}

//============================== WeirdDelay FUNCTIONS ==============================

Apsa23::Apsa23(audioMasterCallback audioMaster)
: AudioEffectX(audioMaster, NUM_PROGRAMS, VDParam_NumParam)    // n program, n parameters
{
    setNumInputs(1);         // Stereo in
    setNumOutputs(1);        // Stereo out
    setUniqueID('GLAA');     // identify
    InitPlugin();
}

void Apsa23::InitPlugin(){
    
    m_Fuzz        = 0.1;
    m_Chorus      = 0.0;
    m_AutoTremolo = 0.0;
    m_Ambient     = 0.0;
    m_Tone        = 0.5;
    m_DryWet      = 1.0;
    
    init( getSampleRate() );
}

void Apsa23::init(float _sampleRate){
        
    O_Chorus.init( _sampleRate );
    O_Tone.init( _sampleRate );
    O_AutoTremolo.init( _sampleRate );
    O_Ambient.init( _sampleRate );
}

void Apsa23::setSampleRate (float _sampleRate){
    AudioEffect::setSampleRate(_sampleRate);
    init( _sampleRate );
}

Apsa23::~Apsa23() {   }

//============================== VST PROCESSING ==============================

void Apsa23::processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames){
    
    // PROCESS SINGLE PRECISION
    float *in  = inputs[0];  // buffer input
    float *out = outputs[0]; // buffer output
    
    // Copy input samples in the output buffer
    for(int i=0; i<sampleFrames; ++i)
        out[i] = in[i];
    
    if(m_Tone != 0.5)
        O_Tone.Process(out, sampleFrames);
    
    if(m_Fuzz != 0.0)
        O_Fuzz.Process(out, sampleFrames);
    
    if(m_Chorus != 0.0)
        O_Chorus.Process(out, sampleFrames);
    
    if(m_Ambient != 0.0)
        O_Ambient.Process(out, sampleFrames);
    
    if(m_AutoTremolo != 0.0)
        O_AutoTremolo.Process(out, in, sampleFrames);

    // Dry/Wet
    for(int i=0; i<sampleFrames; ++i)
        
        out[i] = in[i] * (1.f-m_DryWet) + out[i] * m_DryWet;

}

// ============================== VST INTERFACE ==============================

void Apsa23::setParameter (VstInt32 index, float value)
{
    switch (index) {
        case VDParam_Fuzz:
            m_Fuzz = value;
            O_Fuzz.SetGain( m_Fuzz );
            break;
        
        case VDParam_Chorus:
            m_Chorus = value;
            O_Chorus.SetAmount(m_Chorus);
            break;
            
        case VDParam_AutoTremolo:
            m_AutoTremolo = value;
            O_AutoTremolo.SetDepth( m_AutoTremolo );
            break;
            
        case VDParam_Ambient:
            O_Ambient.SetAmount( m_Ambient );
            m_Ambient = value;
            break;
            
        case VDParam_Tone:
            m_Tone = value;
            O_Tone.SetAmount( m_Tone );
            break;
        
        case VDParam_DryWet:
            m_DryWet = value;
            break;

        default:
            break;
    }
}

float Apsa23::getParameter (VstInt32 index)
{
    float toReturn = 0;

    switch (index) {
        case VDParam_Fuzz:
            toReturn = m_Fuzz;
            break;
            
        case VDParam_Chorus:
            toReturn = m_Chorus;
            break;
            
        case VDParam_AutoTremolo:
            toReturn = m_AutoTremolo;
            break;
            
        case VDParam_Ambient:
            toReturn = m_Ambient;
            break;
            
        case VDParam_Tone:
            toReturn = m_Tone;
            break;
            
        case VDParam_DryWet:
            toReturn = m_DryWet;
            break;
            
        default:
            break;
    }
    
    return toReturn;
}

void Apsa23::getParameterLabel (VstInt32 index, char* label)
{
    
    switch (index) {
        case VDParam_Fuzz:
            vst_strncpy(label, "", kVstMaxParamStrLen);
            break;
            
        case VDParam_Chorus:
            vst_strncpy(label, "", kVstMaxParamStrLen);
            break;
            
        case VDParam_AutoTremolo:
            vst_strncpy(label, "", kVstMaxParamStrLen);
            break;
            
        case VDParam_Ambient:
            vst_strncpy(label, "", kVstMaxParamStrLen);
            break;
            
        case VDParam_Tone:
            vst_strncpy(label, "", kVstMaxParamStrLen);
            break;
            
        case VDParam_DryWet:
            vst_strncpy(label, "", kVstMaxParamStrLen);
            break;
            
        default:
            AudioEffect::getParameterLabel(index, label); // nel caso base chiamo la funzione del SDK
            break;
    }
}

void Apsa23::getParameterDisplay (VstInt32 index, char* text)
{
    switch (index) {
        case VDParam_Fuzz:
            int2string( 1 + m_Fuzz*99, text, kVstMaxParamStrLen);
            break;
            
        case VDParam_Chorus:
            int2string( 1 + m_Chorus*99, text, kVstMaxParamStrLen);
            break;
            
        case VDParam_AutoTremolo:
            int2string( 1 + m_AutoTremolo*99, text, kVstMaxParamStrLen);
            break;
        
        case VDParam_Ambient:
            int2string( 1 + m_Ambient*99, text, kVstMaxParamStrLen);
            break;
        
        case VDParam_Tone:
            int2string( 1 + m_Tone*99, text, kVstMaxParamStrLen);
            break;
        
        case VDParam_DryWet:
            if(m_DryWet == 0)
                vst_strncpy( text, "DRY", kVstMaxParamStrLen);
            else if(m_DryWet == 1)
                vst_strncpy( text, "WET", kVstMaxParamStrLen);
            else
                int2string( m_DryWet * 100 , text, kVstMaxParamStrLen);
            break;
            
        default:
            break;
            
    }
}

void Apsa23::getParameterName (VstInt32 index, char* text)
{

    switch (index) {
        case VDParam_Fuzz:
            vst_strncpy(text, "Fuzz", kVstMaxParamStrLen);
            break;
            
        case VDParam_Chorus:
            vst_strncpy(text, "Chorus", kVstMaxParamStrLen);
            break;
            
        case VDParam_AutoTremolo:
            vst_strncpy(text, "A-Trem", kVstMaxParamStrLen);
            break;
            
        case VDParam_Ambient:
            vst_strncpy(text, "Ambi", kVstMaxParamStrLen);
            break;
            
        case VDParam_Tone:
            vst_strncpy(text, "Tone", kVstMaxParamStrLen);
            break;
            
        case VDParam_DryWet:
            vst_strncpy(text, "DryWet", kVstMaxParamStrLen);
            break;
            
        default:
            break;
    }
}

bool Apsa23::getEffectName(char *name){
    
    vst_strncpy(name, "Apsa23", kVstMaxEffectNameLen);
    return true;
}

bool Apsa23::getVendorString(char *name){
    
    vst_strncpy(name, "MIS Gabriele Leva", kVstMaxVendorStrLen);
    return true;
}

bool Apsa23::getProductString(char *name){
 
    vst_strncpy(name, "Apsa", kVstMaxProductStrLen);
    return true;
}
