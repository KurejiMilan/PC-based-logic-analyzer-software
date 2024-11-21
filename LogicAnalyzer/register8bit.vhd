----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    08:23:45 04/19/2022 
-- Design Name: 
-- Module Name:    register8bit - Behavioral 
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


entity register8bit is
    Port ( registerInput : in  STD_LOGIC_VECTOR (7 downto 0);
           registerOutput : out  STD_LOGIC_VECTOR (7 downto 0);
           latchInData : in  STD_LOGIC);
end register8bit;

architecture Behavioral of register8bit is
	signal outBus : std_logic_vector(7 downto 0) := (others => '0');
begin
	registerOutput <= outBus;
	latch_proc: process (latchInData)
					begin
						if rising_edge(latchInData) then
							outBus <= registerInput;
						end if;
					end process;

end Behavioral;

