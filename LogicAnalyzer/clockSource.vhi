
-- VHDL Instantiation Created from source file clockSource.vhd -- 12:38:06 04/19/2022
--
-- Notes: 
-- 1) This instantiation template has been automatically generated using types
-- std_logic and std_logic_vector for the ports of the instantiated module
-- 2) To use this template to instantiate this entity, cut-and-paste and then edit

	COMPONENT clockSource
	PORT(
		masterClock : IN std_logic;
		clock_sel_mux : IN std_logic_vector(2 downto 0);          
		clockOut : OUT std_logic
		);
	END COMPONENT;

	Inst_clockSource: clockSource PORT MAP(
		masterClock => ,
		clockOut => ,
		clock_sel_mux => 
	);


