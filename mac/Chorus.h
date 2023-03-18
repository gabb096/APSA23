#ifndef Chorus_h
#define Chorus_h

#include <memory>
#include <stdio.h>

#define LFO_FREC 0.5
#define MAX_DEL_TIME 0.2

class Chorus{

    float* Buffer;
    float  amount, LfoCounter;
    float  min[4]   = {1.f,  5.f, 7.3, 4.2}; // milliseconds
    float  depth[4] = {10.5, 5.8, 9.f, 3.6}; // milliseconds
    int    WriteIndex, sizeOfDelayLine, sampleRate;
    
    public:
    
    Chorus();
    ~Chorus();
    
    void  SetAmount(float _amount);
    float GetAmount();
    float ProcessSample(float _input);
    void  Process( float* buffer, int sampleFrame);
    void  init(int sample_rate);
    
    float saturation(float _sample);
    float LfoOutput();
    
};

Chorus::Chorus(){
    
    WriteIndex = 0;
    LfoCounter = 0.0;
    sizeOfDelayLine = 1000;
    
    Buffer = NULL;
}

Chorus::~Chorus(){
    
}

void  Chorus::SetAmount(float _amount){
    
    amount = _amount;
}

float Chorus::GetAmount(){
    
    return amount;
}

float Chorus::ProcessSample(float _input){
    
    float output = 0.0;
            
        float lfo = LfoOutput();
        
        // Read from delay line
        for(int j=0; j<4; j++){
          
            // Calculate reading indexs
            float delTime = 4.f * lfo * (1.f - lfo); // Phase to Sine wave

            delTime = (min[j] + delTime * depth[j] ) * (float)sampleRate * 0.001; // in samples

            int readIndex1 = WriteIndex - (int) delTime;

            if( readIndex1 < 0 )
                readIndex1 += sizeOfDelayLine;
        
            int readIndex2 = readIndex1 <= 0 ? sizeOfDelayLine : readIndex1-1;
            
            // Add to the output the linear interpolation with the previews sample
            float a = delTime - (int)delTime;
            output += Buffer[readIndex1] * (1.f - a) + Buffer[readIndex2] * a;
            
            // Change lfo phase for the next reading index
            lfo += 0.2;
            if(lfo > 1.f)
                lfo -= 1.f;
        }
            
        // Update delay line
        Buffer[ WriteIndex ] = _input;
        
        WriteIndex += 1;
        if(WriteIndex >= sizeOfDelayLine)
            WriteIndex = 0;

        output = _input*(1.f - amount) + saturation(output*0.25) * amount;
        
    // Clipping, just in case
    if ( output > 1.f ) return 1.f;
    
    else if (output < -1.f)  return -1.f;
    
    else  return output;
}

void  Chorus::Process( float* buffer, int sampleFrame){
    
    for(int i=0; i<sampleFrame;++i)
        buffer[i] = ProcessSample(buffer[i]);
}

void Chorus::init(int sample_rate){
    
    sampleRate = sample_rate;
    
    sizeOfDelayLine = MAX_DEL_TIME * sampleRate;
    
    if(NULL != Buffer)
        free(Buffer);
    
    /* Inizialize buffer to 0 */
    Buffer = (float*) malloc(sizeOfDelayLine * sizeof(float));
    memset(Buffer, 0, sizeOfDelayLine * sizeof(float));
    
}

float Chorus::saturation(float _sample){
    
    if(_sample < -3 )
        return -1;
    
    else if(_sample > 3 )
        return 1;
    
    else
    {
        float num = _sample * (27.f + _sample * _sample);
        float den = 27.f + 9.f * _sample * _sample;
        
        return num/den;
    }
}

// Sinusoidal lfo approximated using sin(pi*x) = 4x(1-x) in [0,1]
float Chorus::LfoOutput(){ // Unipolar
        
    LfoCounter += LFO_FREC/(float) sampleRate;
    
    if(LfoCounter > 1.f)
        LfoCounter -= 1.f ;
    
    return LfoCounter;
}

#endif /* Chorus_h */
