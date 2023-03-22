
#ifndef Fuzz_h
#define Fuzz_h

#include <math.h>

class Fuzz{
    
    float gain, invGain;
    
    public:
    
    Fuzz();
    ~Fuzz();
    
    void  SetGain(float _gain);
    float GetGain();
    float ProcessSample(float _input);
    void  Process( float* buffer, int sampleFrame);

};

Fuzz::Fuzz(){

}

Fuzz::~Fuzz(){
    
}

void  Fuzz::SetGain(float _gain){
    
    gain    = pow(10.f, _gain*2); // 0 to 20 dB gain
    invGain =  pow(10.f, -1.4 * _gain);
}

float Fuzz::GetGain(){
    
    return gain;
}

float Fuzz::ProcessSample(float _input){
    
    float output = 0.f;
    
    _input *= gain;
    
    if(_input < -1.f )
        output = -1.f;
    
    else if(_input < 0.f )
        output = _input;
    
    else if(_input < 3 )
    {
        float num = _input * (27.f + _input * _input);
        float den = 27.f + 9.f * _input * _input;
        
        output = num/den;
    }
    else
        output = 1.f;
    
    return output * invGain;
}

void  Fuzz::Process( float* buffer, int sampleFrame){
    
    for(int i=0; i<sampleFrame;++i)
        buffer[i] = ProcessSample(buffer[i]);
}

#endif /* Fuzz_h */
