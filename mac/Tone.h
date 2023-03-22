#ifndef Tone_h
#define Tone_h

#define SHELF_FREQ 750.0
#define GAIN_DB 15.0

#import <math.h>

// Low shelf and high shelf filters using the Audio Biquad Filter Designs

class Tone{
    
    float amount;
    
    float lowCoeff[3];
    float highCoeff[3];
    
    float x_n1, y_n1;
    float c_0;

    int   sampleRate;
    
    public:
    
    void  SetAmount(float _Amount);
    float GetAmount();
    float ProcessSample(float _input);
    void  Process( float* buffer, int sampleFrame);
    void  init(int sampleRate);

    float LowShelf(float _input);
    float HighShelf(float _input);

};

void  Tone::SetAmount(float _Amount){
    
    amount = _Amount;
}

float Tone::GetAmount(){
    
    return amount;
}

float Tone::ProcessSample(float _input){
    
    float output = 0.f;
    
    if(amount == 0.5){
        output = _input;
        x_n1 = 0.f;
        y_n1 = 0.f;
    }
    else if(amount < 0.5){
        float a = 1.f - (amount * 2.f);
        output =  LowShelf(_input)  * a + (1.f - a) * _input;
    }
    else {
        float a = 2.f * (amount - 0.5);
        output =  HighShelf(_input) * a + (1.f - a) * _input;
    }
    
    return output;
}

void  Tone::Process( float* buffer, int sampleFrame){
    
    for(int i=0; i<sampleFrame;++i)
        buffer[i] = ProcessSample(buffer[i]);
}

void  Tone::init(int _sampleRate){
    
    // Setting sample rate
    sampleRate = _sampleRate;
    
    // Coefficients based on Will C. Pirkle - Design audio effect plugin in C++
    float teta = tan(M_PI * SHELF_FREQ/sampleRate);
    float mu = pow(10.0, GAIN_DB * 0.05);
    c_0 = mu - 1.0;

    // Low shelf
    float low_beta = 4.f / (1.f + mu);
    float low_delta = teta * low_beta;
    float low_gamma = (1.f - low_delta)/(1.f + low_delta);

    lowCoeff[0] = (1.f - low_gamma) * 0.5;//a_0
    lowCoeff[1] = lowCoeff[0];//a_1
    lowCoeff[2] = -low_gamma;//b_1
   
    // High shelf
    float high_beta = (1.f + mu)*0.25;
    float high_delta = teta * high_beta;
    float high_gamma = (1.f - high_delta)/(1.f + high_delta);
    
    highCoeff[0] = (1.f + high_gamma) * 0.5;//a_0
    highCoeff[1] = -highCoeff[0];//a_1
    highCoeff[2] = -low_gamma;//b_1
   
}


float Tone::LowShelf(float _input){


    y_n1 = lowCoeff[0] * _input + lowCoeff[1] * x_n1 - lowCoeff[2] * y_n1;
    x_n1 = _input;
    
    return y_n1 * c_0 + _input;

}


float Tone::HighShelf(float _input){

    y_n1 = highCoeff[0] * _input + highCoeff[1] * x_n1 - highCoeff[2] * y_n1;
    x_n1 = _input;
    
    return y_n1 * c_0 + _input;
}


#endif /* Tone_h */
