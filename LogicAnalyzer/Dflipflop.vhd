----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    10:38:14 04/19/2022 
-- Design Name: 
-- Module Name:    Dflipflop - Behavioral 
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


entity Dflipflop is
    Port ( D : in  STD_LOGIC;
           Q : out  STD_LOGIC;
           Qnot : out  STD_LOGIC;
           dclkin : in  STD_LOGIC);
end Dflipflop;

architecture Behavioral of Dflipflop is
	signal doutput : std_logic := '0';
begin
	Q <= doutput;
	Qnot <= not doutput;
	
	dproc: process(dclkin)
	begin
		if falling_edge(dclkin) then
			doutput <= D;
		end if;
	end process;

end Behavioral;

