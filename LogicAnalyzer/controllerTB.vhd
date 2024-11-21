--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:   16:13:47 04/19/2022
-- Design Name:   
-- Module Name:   D:/FpgaProjects/LogicAnalyzer/controllerTB.vhd
-- Project Name:  LogicAnalyzer
-- Target Device:  
-- Tool versions:  
-- Description:   
-- 
-- VHDL Test Bench Created by ISE for module: LogicAnalyzerController
-- 
-- Dependencies:
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
--
-- Notes: 
-- This testbench has been automatically generated using types std_logic and
-- std_logic_vector for the ports of the unit under test.  Xilinx recommends
-- that these types always be used for the top-level I/O of a design in order
-- to guarantee that the testbench will bind correctly to the post-implementation 
-- simulation model.
--------------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
use ieee.std_logic_unsigned.all;
 
ENTITY controllerTB IS
END controllerTB;
 
ARCHITECTURE behavior OF controllerTB IS 
 
    -- Component Declaration for the Unit Under Test (UUT)
 
    COMPONENT LogicAnalyzerController
    PORT(
         enable_latch : IN  std_logic;
         write_enable : IN  std_logic_vector(0 downto 0);
         reset_counter : IN  std_logic;
         master_control : IN  std_logic;
         reset_trigger : IN  std_logic;
         inc : IN  std_logic;
         latch_pulse : IN  std_logic;
         trigger_mux_select : IN  std_logic;
         pulsein_demux_select : IN  std_logic_vector(1 downto 0);
         clock_in : IN  std_logic;
         clock_select_clock_in : IN  std_logic;
         dp : IN  std_logic_vector(7 downto 0);
         dataFromMcu : IN  std_logic_vector(7 downto 0);
         dout : OUT  std_logic_vector(7 downto 0);
         complete : OUT  std_logic;
         trigger_status : OUT  std_logic
        );
    END COMPONENT;
    

   --Inputs
   signal enable_latch : std_logic := '0';
   signal write_enable : std_logic_vector(0 downto 0) := (others => '0');
   signal reset_counter : std_logic := '0';
   signal master_control : std_logic := '0';
   signal reset_trigger : std_logic := '0';
   signal inc : std_logic := '0';
   signal latch_pulse : std_logic := '0';
   signal trigger_mux_select : std_logic := '0';
   signal pulsein_demux_select : std_logic_vector(1 downto 0) := (others => '0');
   signal clock_in : std_logic := '0';
   signal clock_select_clock_in : std_logic := '0';
   signal dp : std_logic_vector(7 downto 0) := (others => '0');
   signal dataFromMcu : std_logic_vector(7 downto 0) := (others => '0');

 	--Outputs
   signal dout : std_logic_vector(7 downto 0);
   signal complete : std_logic;
   signal trigger_status : std_logic;

   -- Clock period definitions
   constant clock : time := 10 ns;
 
BEGIN
 
	-- Instantiate the Unit Under Test (UUT)
   uut: LogicAnalyzerController PORT MAP (
          enable_latch => enable_latch,
          write_enable => write_enable,
          reset_counter => reset_counter,
          master_control => master_control,
          reset_trigger => reset_trigger,
          inc => inc,
          latch_pulse => latch_pulse,
          trigger_mux_select => trigger_mux_select,
          pulsein_demux_select => pulsein_demux_select,
          clock_in => clock_in,
          clock_select_clock_in => clock_select_clock_in,
          dp => dp,
          dataFromMcu => dataFromMcu,
          dout => dout,
          complete => complete,
          trigger_status => trigger_status
        );

   -- Clock process definitions
	clock_in <= not clock_in after clock;

	
	process
	begin
		if dp = "00001111" then
			dp <= "00000000";
		else
			dp <= dp+1;
		end if;
		wait for 1.5 us;
	end process;
	
   -- Stimulus process
   stim_proc: process
   begin		
      
      wait for 10 ns;
		-- select word trigger
		trigger_mux_select <= '0';
		wait for 5 ns;
		
		-- compare register msb
		dataFromMcu <= "00000011";
		wait for 5 ns;
		pulsein_demux_select <= "00";
		wait for 5 ns;
		latch_pulse <= '1';
		wait for 20 ns;
		latch_pulse <= '0';
		
		-- compare register lsb
		dataFromMcu <= "11111111";
		wait for 5 ns;
		pulsein_demux_select <= "01";
		wait for 5 ns;
		latch_pulse <= '1';
		wait for 20 ns;
		latch_pulse <= '0'; 
		
		-- wordTrigger 
		dataFromMcu <= "00000100";
		wait for 5 ns;
		pulsein_demux_select <= "10";
		wait for 5 ns;
		latch_pulse <= '1';
		wait for 20 ns;
		latch_pulse <= '0'; 
		
		-- triggerOn
		dataFromMcu <= "00000000";
		wait for 5 ns;
		pulsein_demux_select <= "11";
		wait for 5 ns;
		latch_pulse <= '1';
		wait for 20 ns;
		latch_pulse <= '0'; 
		
		if trigger_status = '1' then
			reset_trigger <= '1';
			wait for 5 ns;
			reset_trigger <= '0';
		end if;
		
		reset_counter <= '1';
		wait for 10 ns;
		reset_counter <= '0';
		
		enable_latch <= '1';
		write_enable <= "1"; 						-- put RAM into write mode
		master_control <= '1';
		
		wait for 10 ns;
		wait until complete = '1';
		
		-- this section is just to RESET the complete signal
		-- this section maybe only needed for simulation
		write_enable <= "0";							-- put RAM into read mode
		enable_latch <= '0';
		wait for 10 ns;
		master_control <= '0';
		
		wait for 10 ns;
		inc <= '1';
		wait for 10 ns;
		reset_counter <= '1';
		
		wait for 50 ns;
		reset_counter <= '0';
		wait for 10 ns;
		inc <= '0';
		
		wait for 10 ns;
		
		enable_latch <= '0';
		
		
		wait for 20 ns;
		
		while complete = '0' loop
			
			inc <= '1';
			wait for 50 ns;
			inc <= '0';
			wait for 50 ns;
			
		end loop;
		
		
		-- this section is used to disable the address counter
		
		if trigger_status = '1' then
			reset_trigger <= '1';
			wait for 5 ns;
			reset_trigger <= '0';
		end if;
		
      wait;
   end process;

END;
