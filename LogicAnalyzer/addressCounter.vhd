----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    10:47:50 04/19/2022 
-- Design Name: 
-- Module Name:    addressCounter - Behavioral 
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

entity addressCounter is
    Port ( reset : in  STD_LOGIC;
           enable_counter : in  STD_LOGIC;
           counterClkIn : in  STD_LOGIC;
           address : out  STD_LOGIC_VECTOR (15 downto 0);
			  testSignalint : out std_logic);
end addressCounter;

architecture Behavioral of addressCounter is
	signal addressCounterOutput :  std_logic_vector (15 downto 0) := (others => '0');
	signal tempcounter : std_logic_vector (7 downto 0) := (others => '0');
	signal tempsig : std_logic := '0';
begin
	address <= addressCounterOutput;
	tempsig <= tempcounter(2);
	testSignalint <= tempsig;
	
	addressCounterProc : process(counterClkIn, reset)
								begin
								
									if reset = '1' then
										addressCounterOutput <= "0000000000000000";
										tempcounter <= (others => '0');
									end if;
								
									if rising_edge(counterClkIn) then
										if enable_counter = '1' then
											if reset = '0' then
												addressCounterOutput <= addressCounterOutput+1;
												tempcounter <= tempcounter + 1;
												--tempsig <= not tempsig;				-- for testing    TESTED OK THE CONTROL FLOW TAKES PLACE
											end if;
										end if;
									end if;
								end process;
								
--	resetProcess : process(reset)
--						begin
--							if reset = '1' then
--								addressCounterOutput <= "0000000000000000";
--								tempcounter <= (others => '0');
--							end if;
--						end process;

end Behavioral;

