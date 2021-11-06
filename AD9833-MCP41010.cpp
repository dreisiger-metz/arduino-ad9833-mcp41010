// =============================================================================
// Filename         : AD9833-MCP41010.cpp
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
#include "AD9833-MCP41010.h"




// =============================================================================
// Method           : AD9833MCP41010ModuleController::AD9833MCP41010ModuleController()
//
// Purpose          : To initialise the controller's variables and I/O pins
//
// Inputs           : [AD9833SelectPin] the Arduino pin used to drive the 
//                        module's AD9833 FSYNC (aka /CS) line
//                    [MCP41010SelectPin] the Arduino pin used to drive the
//                        MCP41010 /CS line
// =============================================================================
AD9833MCP41010ModuleController::AD9833MCP41010ModuleController(byte AD9833SelectPin, byte MCP41010SelectPin) {
  this->p_AD9833SelectPin = AD9833SelectPin;
  this->p_MCP41010SelectPin = MCP41010SelectPin;

  // Note that AD9833::AD9833 calls SPI.Begin() as part of its own setup. Also
  // note that this "initializes the SPI bus by setting SCK, MOSI, and SS to
  // outputs, pulling SCK and MOSI low, and SS high", which 
  p_generator = new AD9833(p_AD9833SelectPin);
  p_generator->Begin();
  this->enable(false);

  // Setup the MCP41010 programmable potentiometer (note that we don't need to
  // call SPI.Begin() as that was already done by the AD9833 constructor)
  pinMode(p_MCP41010SelectPin, OUTPUT);
  digitalWrite(p_MCP41010SelectPin, HIGH);

  // 
  this->waveform(SQUARE_WAVE);
  this->frequency(1000.0);
  this->phase(0.0);
  this->amplitude(1.0);
}


// =============================================================================
// Method           : AD9833MCP41010ModuleController::waveform()
//
// Purpose          : To set a new waveform type
//
// Inputs           : [waveform] the desired waveform type
// =============================================================================
void AD9833MCP41010ModuleController::waveform(WaveformType waveform) {
  p_waveform = waveform;
  p_generator->SetWaveform(p_activeRegister, waveform);
}


// =============================================================================
// Method           : AD9833MCP41010ModuleController::frequency()
//
// Purpose          : To set a new frequency
//
// Inputs           : [frequency] the desired frequency
//
// Returns          : the actual new frequency (which takes the AD9833's clock,
//                    rounding errors and other internal factors into account)
// =============================================================================
float AD9833MCP41010ModuleController::frequency(float frequency) {
  p_generator->SetFrequency(p_activeRegister, frequency);
  p_frequency = p_generator->GetActualProgrammedFrequency(p_activeRegister);

  return p_frequency;
}


// =============================================================================
// Method           : AD9833MCP41010ModuleController::phase()
//
// Purpose          : To set a new phase
//
// Inputs           : [phase] the desired phase
//
// Returns          : the actual new phase (which takes the AD9833's clock,
//                    rounding errors and other internal factors into account)
// =============================================================================
float AD9833MCP41010ModuleController::phase(float phase) {
  p_generator->SetPhase(p_activeRegister, phase);
  p_phase = p_generator->GetActualProgrammedPhase(p_activeRegister);

  return p_phase;
}


// =============================================================================
// Method           : AD9833MCP41010ModuleController::amplitude()
//
// Purpose          : To set a new amplitude
//
// Inputs           : [amplitude] the desired amplitude
//
// Returns          : the actual new amplitude (which takes the waveform type,
//                    and eventually, the frequency response of the module 
//                    into account)
// =============================================================================
float AD9833MCP41010ModuleController::amplitude(float amplitude) {
  p_gainR = (byte) (amplitude * MaxGainR);

  digitalWrite(p_MCP41010SelectPin, LOW);   // set /CE low
  SPI.transfer(B00010001);                  // Tell the chip to set the pot
  SPI.transfer(p_gainR);                    // Tells it the pot position
  digitalWrite(p_MCP41010SelectPin, HIGH);  // raise /CE high again

  return ((float) p_gainR) / MaxGainR;
}


// =============================================================================
// Method           : AD9833MCP41010ModuleController::enable()
//
// Purpose          : Enables or disables the module's output
//
// Inputs           : [enable] the module's desired state
// =============================================================================
void AD9833MCP41010ModuleController::enable(bool enable) {
  p_enabled = enable;
  p_generator->EnableOutput(p_enabled);
}
