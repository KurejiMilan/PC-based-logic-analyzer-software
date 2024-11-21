----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    08:56:22 04/19/2022 
-- Design Name: 
-- Module Name:    LogicAnalyzerController - Behavioral 
-- Project Name: 
-- Target Devices: 
-- Tool versions: 
-- Description: 
--
-- Dependencies: 
--
-- Revision: 
-- Revision 0.01 - File Created
-- Additional Comments: 
--
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;


entity LogicAnalyzerController is
    
	 Port ( enable_latch : in  STD_LOGIC;													-- input from mcu
           write_enable : in  STD_LOGIC_vector( 0 downto 0);						-- input from mcu
           reset_counter : in  STD_LOGIC;													-- input from mcu
           master_control : in  STD_LOGIC;												-- input from mcu
           reset_trigger : in  STD_LOGIC;													-- input from mcu
           inc : in  STD_LOGIC;																-- input from mcu
           latch_pulse : in  STD_LOGIC;													-- input from mcu
           trigger_mux_select : in  STD_LOGIC;											-- input from mcu
           pulsein_demux_select : in  STD_LOGIC_VECTOR (1 downto 0);				-- input from mcu
			  
			  clock_in : in std_logic;															-- input from 50 MHz clock
			  
			  clock_select_clock_in : in std_logic;										-- this is used to update the clock select register input from mcu
			  
			  dp : in std_logic_vector(7 downto 0);										-- input from probe
			  dataFromMcu: in std_logic_vector(7 downto 0);								-- input from mcu
			  
			  dout : out std_logic_vector(7 downto 0); 									-- output to mcu it carries sampled data
           complete : out  STD_LOGIC;														-- output to mcu
           trigger_status : out  STD_LOGIC;												-- output to mcu
			  test_signal : out std_logic);
			  
end LogicAnalyzerController;

architecture Behavioral of LogicAnalyzerController is
	
	-- ram component decleration
	COMPONENT internalRAM
	  PORT (
		 clka : IN STD_LOGIC;
		 wea : IN STD_LOGIC_VECTOR(0 DOWNTO 0);
		 addra : IN STD_LOGIC_VECTOR(15 DOWNTO 0);
		 dina : IN STD_LOGIC_VECTOR(7 DOWNTO 0);
		 douta : OUT STD_LOGIC_VECTOR(7 DOWNTO 0)
	  );
	END COMPONENT;
	
	-- trigger circuit component decleration
	COMPONENT TriggerCircuit
		PORT(
			latchedWord : IN std_logic_vector(7 downto 0);
			maskRegister : IN std_logic_vector(7 downto 0);
			triggerOn : IN std_logic_vector(7 downto 0);
			resetTrigger : IN std_logic;
			triggerSelector : IN std_logic;          
			triggerOut : OUT std_logic
			);
	END COMPONENT;
	
	-- address counter component decleration
	component addressCounter is
    Port ( reset : in  STD_LOGIC;
           enable_counter : in  STD_LOGIC;
           counterClkIn : in  STD_LOGIC;
           address : out  STD_LOGIC_VECTOR (15 downto 0);
			  testSignalint : out std_logic);
	end component;
	
	-- D flipflop component decleration
	COMPONENT Dflipflop
		PORT(
			D : IN std_logic;
			dclkin : IN std_logic;          
			Q : OUT std_logic;
			Qnot : OUT std_logic
			);
	END COMPONENT;
	
	-- 8 bit register decleration
	COMPONENT register8bit
		PORT(
			registerInput : IN std_logic_vector(7 downto 0);
			latchInData : IN std_logic;          
			registerOutput : OUT std_logic_vector(7 downto 0)
			);
	END COMPONENT;
	
	-- clockSource decleration
	COMPONENT clockSource
		PORT(
			masterClock : IN std_logic;
			clock_sel_mux : IN std_logic_vector(2 downto 0);          
			clockOut : OUT std_logic
			);
	END COMPONENT;
	
	signal clk : std_logic;  																		-- this is the output form the clockSource and the clock signal that drives controller
	signal clock_select_mux_register : std_logic_vector(2 downto 0) := "010";		-- register that is used to select the clock source from 5 available clock divide
	
	-- demux output for latch in signal
	signal comparemsb_latchin, comparelsb_latchin, maskreg_latchin ,triggerreg_latchin : std_logic;
	
	-- signal related to input latch
	signal inputLatch : std_logic_vector(7 downto 0) := (others=>'0');
	
	-- signal realted to address comparator
	signal compareaddress_latch_msb : std_logic_vector(7 downto 0) := "00001111";
	signal compareaddress_latch_lsb : std_logic_vector(7 downto 0) := "11111111";
	signal addressMsbXnor : std_logic_vector(7 downto 0);
	signal addressLsbXnor : std_logic_vector(7 downto 0);
	
	-- signal related to trigger
	signal mask_register : std_logic_vector(7 downto 0) := (others=>'0');
	signal trigger_on_register : std_logic_vector(7 downto 0) := (others=>'0');
	
	signal triggerOutSignal : std_logic;
	
	-- intermediate net is used as a bus where various inputs are connected and latched only to the correct register
	signal intermediateNet : std_logic_vector(7 downto 0) := (others=>'0');
	
	-- address counter output
	signal addressOutput : std_logic_vector(15 downto 0);
	
	-- other intermediate signal
	signal ws0, ws1, ws2, ws3, ws4, ws5, ws6 : std_logic := '0';
	
	-- test signal intermediate
	signal testSignalNet : std_logic; 
begin

	addressMsbXnor <= compareaddress_latch_msb xnor addressOutput(15 downto 8);
	addressLsbXnor	<= compareaddress_latch_lsb xnor addressOutput(7 downto 0);
	
	ws6 <= 	addressMsbXnor(0) and addressMsbXnor(1) and addressMsbXnor(2) and addressMsbXnor(3) and 
				addressMsbXnor(4) and addressMsbXnor(5) and addressMsbXnor(6) and addressMsbXnor(7) and
				addressLsbXnor(0) and addressLsbXnor(1) and addressLsbXnor(2) and addressLsbXnor(3) and 
				addressLsbXnor(4) and addressLsbXnor(5) and addressLsbXnor(6) and addressLsbXnor(7);
	
	ws1 <= ws0 and master_control and clk;
	ws2 <= ws1;
	ws3 <= ws1;
	
	--testSignalNet <= addressOutput(0);							-- for testing
	test_signal <= testSignalNet;
	
	ws4 <= ws2 OR inc; 											-- tested
	ws5 <= not ws4;
	
	intermediateNet <= dataFromMcu;
	
	-- trigger signal from trigger circuit
	-- this is also feed as the addres counter enable
	trigger_status <= triggerOutSignal;
	
	-- creating an instance of trigger circuit
	triggerModule: TriggerCircuit PORT MAP(
		latchedWord => inputLatch,
		maskRegister => mask_register,
		triggerOn => trigger_on_register,
		resetTrigger => reset_trigger,
		triggerSelector => trigger_mux_select,
		triggerOut => triggerOutSignal
	);
	-- creating an instance of D flipflop
	DlatchModule: Dflipflop PORT MAP(
		D => ws6,
		Q => complete,
		Qnot => ws0,
		dclkin => ws4
	);
	
	-- creating an instace of clock source  TESTED
	clockSourceModule: clockSource PORT MAP(
		masterClock => clock_in,
		clockOut => clk,
		clock_sel_mux => clock_select_mux_register
	);
	
	-- creating instance of address counter
	addressCounterModule: addressCounter PORT MAP(
		reset => reset_counter,
		enable_counter => triggerOutSignal,
		counterClkIn => ws4, 
		address => addressOutput,
		testSignalint => testSignalNet
	);

	-- creating an instance of different registers
	mask_register_module : register8bit PORT MAP(
		registerInput => intermediateNet,
		registerOutput => mask_register, 
		latchInData => maskreg_latchin 
	);
	
	triggeron_register_module : register8bit PORT MAP(
		registerInput => intermediateNet,
		registerOutput => trigger_on_register, 
		latchInData => triggerreg_latchin 
	);
	
	comparemsb_register_module : register8bit PORT MAP(
		registerInput => intermediateNet,
		registerOutput => compareaddress_latch_msb, 
		latchInData => comparemsb_latchin 
	);
	
	comparelsb_register_module : register8bit PORT MAP(
		registerInput => intermediateNet,
		registerOutput => compareaddress_latch_lsb, 
		latchInData => comparelsb_latchin 
	);
	
	RamModule : internalRAM
	  PORT MAP (
		 clka => ws5,
		 wea => write_enable,
		 addra => addressOutput,
		 dina => inputLatch,
		 douta => dout
	  );
	  
	--pulsein process
	pulseinProc: process(latch_pulse, pulsein_demux_select)
	begin
		case pulsein_demux_select is
			when "00" => comparemsb_latchin <= latch_pulse;
			when "01" => comparelsb_latchin <= latch_pulse;
			when "10" => maskreg_latchin <= latch_pulse;
			when others => triggerreg_latchin <= latch_pulse;
		end case;
	end process;
	
	
	-- process to update the clock select mux register
	clockSelectMuxProc: process (clock_select_clock_in)
	begin
		if rising_edge(clock_select_clock_in) then
			clock_select_mux_register <=  intermediateNet(2 downto 0);
		end if;
	end process;
	
	-- process to read the input data from the probe
	readDataProc : process(ws3) 
						begin
							if rising_edge(ws3) then
								if enable_latch = '1' then
									inputLatch <= dp;
								end if;
							end if;
						end process;

end Behavioral;

