#include <stdio.h>
#include "public.sdk/source/vst2.x/audioeffectx.h"
#include <memory>
#include "Fuzz.h"
#include "Chorus.h"
#include "AutoTremolo.h"
#include "Tone.h"
#include "Filter.hpp"

# define NUM_PROGRAMS 0

using namespace std;

enum Apsa23Param{
    
    VDParam_Fuzz,
    VDParam_Chorus,
    VDParam_Tone,
    VDParam_AutoTremolo,
    VDParam_DryWet,
    VDParam_NumParam
};


//====================================================================

class Apsa23 : public AudioEffectX{

    float m_Fuzz, m_Chorus, m_AutoTremolo, m_Tone, m_DryWet;
    
    Fuzz O_Fuzz;
    Chorus O_Chorus;
    AutoTremolo O_AutoTremolo;
    Tone O_Tone;
    Filter O_LowPass;
    
    void InitPrograms();
    void InitPlugin();

public:

    Apsa23(audioMasterCallback audioMaster);
    ~Apsa23();
    
    void init(float _sampleRate);
    
    virtual void processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames) override;
    //virtual void processDoubleReplacing(double** inputs, double** outputs, VstInt32 sampleFrames) override;
    
    virtual void setSampleRate (float sampleRate) override;
    virtual void setParameter (VstInt32 index, float value) override;
    virtual float getParameter (VstInt32 index) override;
    virtual void getParameterLabel (VstInt32 index, char* label) override;
    virtual void getParameterDisplay (VstInt32 index, char* text) override;
    virtual void getParameterName (VstInt32 index, char* text) override;
    virtual bool getEffectName(char* name) override;
    virtual bool getVendorString(char* name) override;
    virtual bool getProductString(char* name) override;
    
};
