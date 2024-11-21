----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    21:12:47 04/18/2022 
-- Design Name: 
-- Module Name:    TriggerCircuit - Behavioral 
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


entity TriggerCircuit is
    Port ( latchedWord : in  STD_LOGIC_VECTOR (7 downto 0);
           maskRegister : in  STD_LOGIC_VECTOR (7 downto 0);
           triggerOn : in  STD_LOGIC_VECTOR (7 downto 0);
           resetTrigger : in  STD_LOGIC;
           triggerSelector : in  STD_LOGIC;
			  triggerOut : out STD_LOGIC);
end TriggerCircuit;

architecture Behavioral of TriggerCircuit is
	
	component triggerTflipflop is
    Port ( toggle : in  STD_LOGIC;
           Q : out  STD_LOGIC;
           Qnot : out  STD_LOGIC);
	end component;

	signal mskOut0, mskOut1, mskOut2, mskOut3, mskOut4, mskOut5, mskOut6, mskOut7 : std_logic;
	signal edgeXorOut0, edgeXorOut1, edgeXorOut2, edgeXorOut3, edgeXorOut4, edgeXorOut5, edgeXorOut6, edgeXorOut7 : std_logic;
	signal wordXnorOut0, wordXnorOut1, wordXnorOut2, wordXnorOut3, wordXnorOut4, wordXnorOut5, wordXnorOut6, wordXnorOut7 : std_logic;
	signal wordTrigger, edgeTrigger : std_logic;
	signal triggerMuxOut : std_logic; 
	
	signal s1, s2, s3 : std_logic;
begin
	
	Tflipflop0 : triggerTflipflop port map(toggle => s1, Q=> triggerOut, Qnot=> s2);
	
	-- the masked output
	mskOut0 <= latchedWord(0) and maskRegister(0); 
	mskOut1 <= latchedWord(1) and maskRegister(1); 
	mskOut2 <= latchedWord(2) and maskRegister(2); 
	mskOut3 <= latchedWord(3) and maskRegister(3); 
	mskOut4 <= latchedWord(4) and maskRegister(4); 
	mskOut5 <= latchedWord(5) and maskRegister(5); 
	mskOut6 <= latchedWord(6) and maskRegister(6); 
	mskOut7 <= latchedWord(7) and maskRegister(7);

	-- edge trigger xor section
	edgeXorOut0 <= mskOut0 xor triggerOn(0);
	edgeXorOut1 <= mskOut1 xor triggerOn(1);
	edgeXorOut2 <= mskOut2 xor triggerOn(2);
	edgeXorOut3 <= mskOut3 xor triggerOn(3);
	edgeXorOut4 <= mskOut4 xor triggerOn(4);
	edgeXorOut5 <= mskOut5 xor triggerOn(5);
	edgeXorOut6 <= mskOut6 xor triggerOn(6);
	edgeXorOut7 <= mskOut7 xor triggerOn(7);
	
	edgeTrigger <=  edgeXorOut0 or edgeXorOut1 or edgeXorOut2 or edgeXorOut3 or edgeXorOut4 or edgeXorOut5 or edgeXorOut6 or edgeXorOut7;
	
	-- word trigger xnor secton
	wordXnorOut0 <= mskOut0 xnor triggerOn(0);
	wordXnorOut1 <= mskOut1 xnor triggerOn(1);
	wordXnorOut2 <= mskOut2 xnor triggerOn(2);
	wordXnorOut3 <= mskOut3 xnor triggerOn(3);
	wordXnorOut4 <= mskOut4 xnor triggerOn(4);
	wordXnorOut5 <= mskOut5 xnor triggerOn(5);
	wordXnorOut6 <= mskOut6 xnor triggerOn(6);
	wordXnorOut7 <= mskOut7 xnor triggerOn(7);
	
	wordTrigger <= wordXnorOut0 and wordXnorOut1 and wordXnorOut2 and wordXnorOut3 and wordXnorOut4 and wordXnorOut5 and wordXnorOut6 and wordXnorOut7;
	
	-- mux selector to select the trigger source
	triggerMuxOut <=  edgeTrigger when triggerSelector = '0' else
							wordTrigger;
							
	s3 <= triggerMuxOut and s2;
	s1 <= resetTrigger or s3;
end Behavioral;

