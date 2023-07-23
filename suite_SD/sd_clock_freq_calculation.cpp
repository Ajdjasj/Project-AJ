
#include "sdcard_test.hpp"


class SDClkFreqCalculationTest : public SDCardTest
{
  private:

  public:
    SDClkFreqCalculationTest(const TServerTestFactory &Factory) : SDCardTest(Factory)
    {
    }

    ~SDClkFreqCalculationTest()
    {
    }

    /// Note: derived classes (tests) *must* call this base class version
    /// to get proper setup of display contexts etc.
    virtual void PreRun()
	{
		    SDCardTest::PreRun();
	}

    /// Note: derived classes (tests) *must* call this base class version
    /// to get proper cleanup of display contexts etc.
    virtual void PostRun()
	{
		    SDCardTest::PostRun();
	}

	
    virtual ts::Test::Result Main()
    {
        Result TestResult = ts::Test::Pass;
		uint8_t Devices = 0, Slots = 0;
		//_reg previous_value, written_value, present_value;
		uint32_t present_state = 0, InsertRemoveValue = 0;
		uint32_t int_status = 0;
		
	sd_logmsg("---Testing the DEVIDED Clock Mode Test---\n");

	sd_logmsg("Setting the Freq : 400KHz\n");
	set_clock( 0, 0,  DEFAULT_FREQUENCY , DIVIDED_CLK_SELECTION_MODE);
	sd_logmsg("Setting the Freq : 25MHz\n");
	set_clock( 0, 0,  DS_MAX_FREQ , DIVIDED_CLK_SELECTION_MODE);
	sd_logmsg("Setting the Freq : 50MHz\n");
	set_clock( 0, 0,  HS_MAX_FREQ , DIVIDED_CLK_SELECTION_MODE);
	sd_logmsg("Setting the Freq : 100MHz\n");
	set_clock( 0, 0,  SDR50_MAX_FREQ , DIVIDED_CLK_SELECTION_MODE);
	sd_logmsg("Setting the Freq : 208MHz\n");
	set_clock( 0, 0,  SDR104_MAX_FREQ , DIVIDED_CLK_SELECTION_MODE);
	
	sd_logmsg("\n---Testing the PROGRAMABLE Clock Mode Test---\n");
	sd_logmsg("Setting the Freq : 25MHz\n");
	set_clock( 0, 0,  DS_MAX_FREQ , PROGRAMABLE_CLK_SELECTION_MODE);
	sd_logmsg("Setting the Freq : 50MHz\n");
	set_clock( 0, 0,  HS_MAX_FREQ , PROGRAMABLE_CLK_SELECTION_MODE);
	sd_logmsg("Setting the Freq : 100MHz\n");
	set_clock( 0, 0,  SDR50_MAX_FREQ , PROGRAMABLE_CLK_SELECTION_MODE);
	sd_logmsg("Setting the Freq : 208MHz\n");
	set_clock( 0, 0,  SDR104_MAX_FREQ , PROGRAMABLE_CLK_SELECTION_MODE);		

	return ts::Test::Pass;
	}	
};

TServerTestInstance(SDClkFreqCalculationTest, SDClkFreqCalculationTest);
