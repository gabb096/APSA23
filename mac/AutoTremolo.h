#ifndef AutoTremolo_h
#define AutoTremolo_h

#include <math.h>

#define RMS_BUFFER 1024 //samples
#define MAX_FREQ 20
#define MIN_FREQ 5

class AutoTremolo{
    
    float depth, sampleRate, LfoCounter, inv_T;
    float buffer[RMS_BUFFER];
    int   index;
    float sums;
    
    public:
    
    void  SetDepth(float _depth);
    float GetDepth();
    float ProcessSample(float _input, float _sideChain);
    void  Process( float* buffer, float* sideChain, int sampleFrame);
    void  init(int _sampleRate);
    float EnvelopeFollower(float _input);
    float LfoOutput(float _freq);

};

void  AutoTremolo::SetDepth(float _depth){
    
    depth = _depth;
}

float AutoTremolo::GetDepth(){
    
    return depth;
}

float AutoTremolo::ProcessSample(float _input, float _sideChain){
    
    float freq = MIN_FREQ + EnvelopeFollower(_sideChain) * MAX_FREQ;
    float level = 1.0 - depth * ( 1.f - LfoOutput(freq) );

    return _input * level;
}

void  AutoTremolo::Process( float* buffer, float* sideChain,  int sampleFrame){
    
    for(int i=0; i<sampleFrame;++i)
        buffer[i] = ProcessSample(buffer[i], sideChain[i]);
}

void  AutoTremolo::init(int _sampleRate){
    
    sampleRate = _sampleRate;
    LfoCounter = 0.0;
    index      = 0;
    inv_T      = 1.f/(float)RMS_BUFFER;
    sums       = 0.0;
    
    for(int i = 0; i < RMS_BUFFER; i++)
        buffer[i] = 0.f;
}


float AutoTremolo::EnvelopeFollower(float _input){ // RMS 
    
    float env = _input * _input * inv_T;
    
    sums += env - buffer[index];
    buffer[index] = env;
    
    index ++;
    
    if(index == RMS_BUFFER)
        index = 0;
    
    return powf(sums + 0.01, 0.5) * 2;
}

float AutoTremolo::LfoOutput(float _freq){ // Unipolar
        
    LfoCounter += _freq/(float) sampleRate;
    
    if(LfoCounter > 1.f)
        LfoCounter -= 1.f ;
    
    return 4.f * LfoCounter * (1.f - LfoCounter); // Phase to Sine wave;
}


#endif /* AutoTremolo_h */
