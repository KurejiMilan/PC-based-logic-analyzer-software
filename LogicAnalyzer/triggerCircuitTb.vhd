--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:   21:54:06 04/18/2022
-- Design Name:   
-- Module Name:   D:/FpgaProjects/LogicAnalyzer/triggerCircuitTb.vhd
-- Project Name:  LogicAnalyzer
-- Target Device:  
-- Tool versions:  
-- Description:   
-- 
-- VHDL Test Bench Created by ISE for module: TriggerCircuit
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

--USE ieee.numeric_std.ALL;
 
ENTITY triggerCircuitTb IS
END triggerCircuitTb;
 
ARCHITECTURE behavior OF triggerCircuitTb IS 
 
    -- Component Declaration for the Unit Under Test (UUT)
 
    COMPONENT TriggerCircuit
    PORT(
         latchedWord : IN  std_logic_vector(7 downto 0);
         maskRegister : IN  std_logic_vector(7 downto 0);
         triggerOn : IN  std_logic_vector(7 downto 0);
         resetTrigger : IN  std_logic;
         triggerSelector : IN  std_logic;
         triggerOut : OUT  std_logic
        );
    END COMPONENT;
    

   --Inputs
   signal latchedWord : std_logic_vector(7 downto 0) := (others => '0');
   signal maskRegister : std_logic_vector(7 downto 0) := (others => '0');
   signal triggerOn : std_logic_vector(7 downto 0) := (others => '0');
   signal resetTrigger : std_logic := '0';
   signal triggerSelector : std_logic := '0';

 	--Outputs
   signal triggerOut : std_logic;
   -- No clocks detected in port list. Replace <clock> below with 
   -- appropriate port name 
 
   constant period : time := 10 ns;
 
BEGIN
 
	-- Instantiate the Unit Under Test (UUT)
   uut: TriggerCircuit PORT MAP (
          latchedWord => latchedWord,
          maskRegister => maskRegister,
          triggerOn => triggerOn,
          resetTrigger => resetTrigger,
          triggerSelector => triggerSelector,
          triggerOut => triggerOut
        );
	
	resetTrigger <= '0';
	
	triggerSelector <= '0';
	maskRegister <= "00000010";
	triggerOn <= "00000010";
	
	
	latchProcess: process
	begin
		latchedWord <= latchedWord + 1;
		wait for 100 ns;
	end process;

END;
