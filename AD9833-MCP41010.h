// =============================================================================
// Filename         : AD9833-MCP41010.h
// Version          : 
//
// Original Author  : Peter Metz
// Date Created     : 02-Jun-2020
//
// Revision History : 02-Jun-2020: Initial version
//
// Purpose          : 
//                     
// Licensing        : Copyright (C) 2020, Peter Metz
//
// This program is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more 
// details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
//
// ----------------------------------------------------------------------------
// Notes:
// =============================================================================
#ifndef AD9833_MCP41010_h
#define AD9833_MCP41010_h
#include <AD9833.h>




// =============================================================================
// Class            : AD9833MCP41010ModuleController
//
// Purpose          : This class implements the controller for the combined 
//                    AD9833 / MCP41010 module.
// =============================================================================
class AD9833MCP41010ModuleController {
  public:
    AD9833MCP41010ModuleController(byte AD9833SelectPin, byte MCP41010SelectPin);
    ~AD9833MCP41010ModuleController() { enable(false); delete p_generator; }

    void  waveform(WaveformType waveform);
    float frequency(float frequency);
    float phase(float phase);
    float amplitude(float amplitude);
    void  enable(bool enable);

    inline WaveformType waveform() { return p_waveform; }
    inline float frequency() { return p_frequency; }
    inline float phase()     { return p_phase; }
    inline float amplitude() { return ((float) p_gainR) / MaxGainR; }
    inline bool  enable()    { return p_enabled; }

  protected:
    static const Registers p_activeRegister = REG0;  // either REG0 or REG1
    static const byte MaxGainR = 255;   // MCP41010 values corresponding the maximum actual amplification values

    byte         p_AD9833SelectPin, p_MCP41010SelectPin;
    float        p_frequency, p_phase;  // frequency is between 0 and 12.5 MHz
    WaveformType p_waveform;            // one of SINE_WAVE, TRIANGLE_WAVE, SQUARE_WAVE or HALF_SQUARE_WAVE
    byte         p_gainR;
    bool         p_enabled;
    AD9833      *p_generator;
};

#endif
