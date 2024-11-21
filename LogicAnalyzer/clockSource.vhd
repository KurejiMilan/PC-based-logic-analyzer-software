----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    12:25:51 04/19/2022 
-- Design Name: 
-- Module Name:    clockSource - Behavioral 
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

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity clockSource is
    Port ( masterClock : in  STD_LOGIC;
           clockOut : out  STD_LOGIC;
           clock_sel_mux : in  STD_LOGIC_VECTOR (2 downto 0));
end clockSource;
	
architecture Behavioral of clockSource is
	signal clockSourceCounter : std_logic_vector(6 downto 0) := (others=> '0');
begin
	-- mux section
	clockOut <= clockSourceCounter(2) when clock_sel_mux = "000" else
					clockSourceCounter(3) when clock_sel_mux = "001" else
					clockSourceCounter(4) when clock_sel_mux = "010" else
					clockSourceCounter(5) when clock_sel_mux = "011" else
					clockSourceCounter(6);
					
	-- up counter process
	upCounterProc : process(masterClock)
	begin
		if rising_edge(masterClock) then
			clockSourceCounter <= clockSourceCounter+1;
		end if;
	end process;
end Behavioral;

