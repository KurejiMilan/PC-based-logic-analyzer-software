
-- VHDL Instantiation Created from source file register8bit.vhd -- 12:12:27 04/19/2022
--
-- Notes: 
-- 1) This instantiation template has been automatically generated using types
-- std_logic and std_logic_vector for the ports of the instantiated module
-- 2) To use this template to instantiate this entity, cut-and-paste and then edit

	COMPONENT register8bit
	PORT(
		registerInput : IN std_logic_vector(7 downto 0);
		latchInData : IN std_logic;          
		registerOutput : OUT std_logic_vector(7 downto 0)
		);
	END COMPONENT;

	Inst_register8bit: register8bit PORT MAP(
		registerInput => ,
		registerOutput => ,
		latchInData => 
	);


