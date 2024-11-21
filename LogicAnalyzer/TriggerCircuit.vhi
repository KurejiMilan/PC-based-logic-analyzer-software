
-- VHDL Instantiation Created from source file TriggerCircuit.vhd -- 08:57:08 04/19/2022
--
-- Notes: 
-- 1) This instantiation template has been automatically generated using types
-- std_logic and std_logic_vector for the ports of the instantiated module
-- 2) To use this template to instantiate this entity, cut-and-paste and then edit

	COMPONENT TriggerCircuit
	PORT(
		latchedWord : IN std_logic_vector(7 downto 0);
		maskRegister : IN std_logic_vector(7 downto 0);
		triggerOn : IN std_logic_vector(7 downto 0);
		resetTrigger : IN std_logic;
		triggerSelector : IN std_logic;          
		triggerOut : OUT std_logic
		);
	END COMPONENT;

	Inst_TriggerCircuit: TriggerCircuit PORT MAP(
		latchedWord => ,
		maskRegister => ,
		triggerOn => ,
		resetTrigger => ,
		triggerSelector => ,
		triggerOut => 
	);


