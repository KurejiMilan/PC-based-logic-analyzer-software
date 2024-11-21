
-- VHDL Instantiation Created from source file addressCounter.vhd -- 14:35:07 04/19/2022
--
-- Notes: 
-- 1) This instantiation template has been automatically generated using types
-- std_logic and std_logic_vector for the ports of the instantiated module
-- 2) To use this template to instantiate this entity, cut-and-paste and then edit

	COMPONENT addressCounter
	PORT(
		reset : IN std_logic;
		enable_counter : IN std_logic;
		counterClkIn : IN std_logic;          
		address : OUT std_logic_vector(15 downto 0)
		);
	END COMPONENT;

	Inst_addressCounter: addressCounter PORT MAP(
		reset => ,
		enable_counter => ,
		counterClkIn => ,
		address => 
	);


