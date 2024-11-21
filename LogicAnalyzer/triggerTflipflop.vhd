
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;


entity triggerTflipflop is
    Port ( toggle : in  STD_LOGIC;
           Q : out  STD_LOGIC;
           Qnot : out  STD_LOGIC);
end triggerTflipflop;

architecture Behavioral of triggerTflipflop is
	signal tQout : std_logic := '0';
begin
	
	Q <= tQout;
	Qnot <= not tQout;
	
	toggleProcess: process(toggle)
	begin
		if rising_edge(toggle) then
			tQout <= not tQout;
		end if;
	end process;
end Behavioral;

