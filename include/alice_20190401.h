# ifndef ALICE_H_
# define ALICE_H_

# include "../include/netxpto_20180815.h"

# include "../include/binary_source_20180815.h"
# include "../include/quantum_framer_20180815.h"
# include "../include/m_qam_mapper_20180815.h"
# include "../include/discrete_to_continuous_time_20180815.h"
# include "../include/sink_20180815.h"
# include "../include/pulse_shaper_20180815.h"
# include "../include/time_delay_20180815.h"
# include "../include/single_photon_source_20180815.h"
# include "../include/polarizer_20180815.h"
# include "../include/sink_20180815.h"

class Alice : public SuperBlock {

public:

	//##############################################################################################################

	Alice(initializer_list<Signal *> inputSig, initializer_list<Signal *> outputSig);

	void initialize(void);
	bool runBlock(void);

	bool firstTime{ true };

	//##############################################################################################################

	void setFrameLength(t_integer fLength) { QuantumFramer_.setFrameLength(fLength); };
	t_integer const getFrameLength(void) { return QuantumFramer_.getFrameLength(); };

	void setHeaderLength(t_integer hLength) { QuantumFramer_.setHeaderLength(hLength); };
	t_integer const getHeaderLength(void) { return QuantumFramer_.getHeaderLength(); };

	void setM(int mValue) { MQamMapper_.setM(mValue); };
	void setIqAmplitudes(vector<vector<t_real>> iqAmplitudesValues) { MQamMapper_.setIqAmplitudes(iqAmplitudesValues); };

	void setFirstTime(bool fTime) { MQamMapper_.setFirstTime(fTime); }; 
	bool getFirstTime() { return MQamMapper_.getFirstTime(); };

	void setNumberOfSamplesPerSymbol(int nSamplesPerSymbol) { DiscreteToContinuousTime_.setNumberOfSamplesPerSymbol(nSamplesPerSymbol); };

	int const getNumberOfSamplesPerSymbol(void) { return DiscreteToContinuousTime_.getNumberOfSamplesPerSymbol(); };

	void setImpulseResponseTimeLength(int impResponseTimeLength) { PulseShaper_.setImpulseResponseTimeLength(impResponseTimeLength); };
	int const getImpulseResponseTimeLength(void) { return PulseShaper_.getImpulseResponseTimeLength(); };

	void setFilterType(pulse_shapper_filter_type fType) { PulseShaper_.setFilterType(fType); };
	pulse_shapper_filter_type const getFilterType(void) { return PulseShaper_.getFilterType(); };

	void setRollOffFactor(double rOffFactor) { PulseShaper_.setRollOffFactor(rOffFactor); };
	double const getRollOffFactor() { return PulseShaper_.getRollOffFactor(); };

	void setPulseWidth(double pWidth) { PulseShaper_.setPulseWidth(pWidth); };
	double const getPulseWidth() { return PulseShaper_.getPulseWidth(); };

	void setTimeDelay(t_real tDelay) { TimeDelay_.setTimeDelay(tDelay); };

	t_real const getTimeDelay(void) { return TimeDelay_.getTimeDelay(); };

	void setNumberOfSymbols(long int nSymbols) { Sink_MQM_Alice_bits.setNumberOfSamples((nSymbols * 16) - (MQM_bits.getBufferLength())); };

	//###############################################################################################################

private:
	/*Input Parameters*/
	string signalsFolderName{ "signals/SuperBlock_Alice" };
	bool logValue{ true };
	string logFileName{ "SuperBlock_Alice.txt" };

	Binary BS_in_1{ "BS_in_1.sgn" }, BS_in_2{ "BS_in_2.sgn" };
	Binary QF_out{ "QF_out.sgn" , 32 };
	QuantumFramer QuantumFramer_{ { &BS_in_1, &BS_in_2 },{ &QF_out } };
	
	TimeDiscreteAmplitudeContinuousReal MQM_out{ "MQM_out.sgn" }, MQM_basis{ "MQM_basis.sgn" }, MQM_bits{ "MQM_bits.sgn" }, MQM_clk{ "MQM_clk.sgn" };
	MQamMapper MQamMapper_{ { &QF_out },{ &MQM_out, &MQM_basis, &MQM_bits, &MQM_clk } };
	// the in-phase componente is used to select both the basis and the bit, 00 => 0 (basis=0, bit=0), 01 => PI/2, 10 => -PI/4, 11 => PI/4
	// the quadrature componente is used as a clock

	TimeContinuousAmplitudeContinuousReal DTCT_basis{ "DTCT_out_basis.sgn" };
	DiscreteToContinuousTime DiscreteToContinuousTime_Basis{ {&MQM_basis},{&DTCT_basis} };

	TimeContinuousAmplitudeContinuousReal Alice_out_0{ "Alice_out_0.sgn" };
	PulseShaper PulseShaper_Basis{ {&DTCT_basis},{&Alice_out_0} };

	Sink Sink_MQM_Alice_basis{ { &Alice_out_0 },{} };

	TimeContinuousAmplitudeContinuousReal DTCT_bits{ "DTCT_bits.sgn" };
	DiscreteToContinuousTime DiscreteToContinuousTime_Bits{ {&MQM_bits},{&DTCT_bits} };

	TimeContinuousAmplitudeContinuousReal Alice_out_1{ "Alice_out_1.sgn" };
	PulseShaper PulseShaper_Bits{ {&DTCT_bits},{&Alice_out_1} };

	Sink Sink_MQM_Alice_bits{ { &Alice_out_1 },{} };

	TimeContinuousAmplitudeContinuousReal DTCT_out{ "DTCT_out.sgn" };
	DiscreteToContinuousTime DiscreteToContinuousTime_{ { &MQM_out },{ &DTCT_out } };
	
	TimeContinuousAmplitudeContinuousReal PS_out{ "PS_out.sgn" };
	PulseShaper PulseShaper_{ { &DTCT_out },{ &PS_out } };
	
	TimeContinuousAmplitudeContinuousReal DTCT_clk{ "DTCT_clk.sgn" };
	DiscreteToContinuousTime DiscreteToContinuousTime_1{ { &MQM_clk },{ &DTCT_clk } };
	
	TimeContinuousAmplitudeContinuousReal TD_clk{ "TD_clk.sgn" };
	TimeDelay TimeDelay_{ { &DTCT_clk },{ &TD_clk } };
	
	PhotonStreamXY SPS_out{ "SPS_out.sgn" };
	SinglePhotonSource SinglePhotonSource_{ { &TD_clk },{ &SPS_out } };

	PhotonStreamXY Alice_out_2{ "P_out.sgn" };
	Polarizer Polarizer_{ { &SPS_out, &PS_out },{ &Alice_out_2 } };

};

# endif


