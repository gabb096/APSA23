#ifndef Ambient_h
#define Ambient_h

#include <memory>
#include <stdio.h>
#include <cmath>
#include <cstring>

#define GAIN  0.86

class Ambient{

    float* buffer_1;            // comb filter 1
    float sizeOfDelayLine_1;
    float index_1;
    
    float* buffer_2;            // comb filter 2
    float sizeOfDelayLine_2;
    float index_2;
    
    float* buffer_3;            // comb filter 3
    float sizeOfDelayLine_3;
    float index_3;
    
    float* buffer_4;            // comb filter 4
    float sizeOfDelayLine_4;
    float index_4;

    float amount;
    int sampleRate;
    
    public:
    
    Ambient();
    ~Ambient();
    
    void  SetAmount(float _amount);
    float GetAmount();
    float ProcessSample(float _input);
    void  Process( float* buffer, int sampleFrame);
    void  init(int sample_rate);
    void  initBuffer(float* (&buffer), float sizeOfDelayLine);
    float processCombFilter(float* (&buffer), float _input, float gain, float sizeOfDelayLine, float  & index);
    float saturation(float _sample);
};

Ambient::Ambient(){
    
    buffer_1 = NULL;
    buffer_2 = NULL;
    buffer_3 = NULL;
    buffer_4 = NULL;
}

Ambient::~Ambient(){
    
}

void Ambient::SetAmount(float _amount){
    
    amount = _amount;
}

float Ambient::GetAmount(){
    
    return amount;
}

float Ambient::ProcessSample(float _input){
    
    float output = 0.0;
    float input = amount * _input;
    
    /* 4 comb filters in parallel */
    output =  processCombFilter(buffer_1, input,  GAIN, sizeOfDelayLine_1, index_1);

    output += processCombFilter(buffer_2, input, -GAIN, sizeOfDelayLine_2, index_2);

    output += processCombFilter(buffer_3, input,  GAIN, sizeOfDelayLine_3, index_3);

    output += processCombFilter(buffer_4, input, -GAIN, sizeOfDelayLine_4, index_4);

    output = saturation(output * 0.25); // saturation and "gain control"
    
    return _input + output;
}

void Ambient::Process( float* buffer, int sampleFrame){
    
    for(int i=0; i<sampleFrame;++i)
        buffer[i] = ProcessSample(buffer[i]);
}

void Ambient::init(int sample_rate){
    
    sampleRate = sample_rate;
    
    index_1 = 0;
    sizeOfDelayLine_1 = 1.9 * 0.001 * sampleRate;// 1st comb filter has a  1.6 ms delay line
    initBuffer(buffer_1, sizeOfDelayLine_1);
    
    index_2 = 0;
    sizeOfDelayLine_2 = 3.4 * 0.001 * sampleRate;// 2nd comb filter has a  7.4 ms delay line
    initBuffer(buffer_2, sizeOfDelayLine_2);
    
    index_3 = 0;
    sizeOfDelayLine_3 = 6.5 * 0.001 * sampleRate;// 3rd comb filter has a 13.5 ms delay line
    initBuffer(buffer_3, sizeOfDelayLine_3);
    
    index_4 = 0;
    sizeOfDelayLine_4 = 8.3 * 0.001 * sampleRate;// 4th comb filter has a 21.7 ms delay line
    initBuffer(buffer_4, sizeOfDelayLine_4);
    
}

void Ambient::initBuffer(float* (&buffer), float sizeOfDelayLine){
        
    if(NULL != buffer)
        free(buffer);
        
    int m = ceil(sizeOfDelayLine * sizeof(float));
    
    buffer = (float*) malloc( m );
    memset(buffer, 0, m);
}

float Ambient::processCombFilter(float* (&buffer), float _input, float gain, float sizeOfDelayLine, float  & index){
    
    float output = 0.0;
        
    int readIndex1 = index;
        
    float a = index - readIndex1;

    int readIndex2 = readIndex1 + 1;
        
    if(readIndex2 >= sizeOfDelayLine)
        readIndex2 = 0;
        
    output = buffer[readIndex1] * (1.f - a) + buffer[readIndex2] * a;
        
    buffer[readIndex1] = _input + gain * output;
        
    if(buffer[readIndex1] < 0.00001) // set to 0 if value is at -50dBFS
        buffer[readIndex1] = 0.f;
        
    index = index + 1;
    
    if( index >= sizeOfDelayLine )
        index -= sizeOfDelayLine;
        
        return output;
}

float Ambient::saturation(float _sample){
    
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

#endif /* Ambient_h */
