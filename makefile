all:
	g++ --std=c++11 main.cpp ./REtoNFA/classes.cpp ./REtoNFA/REtoNFA.cpp ./mssc/dfArrow.cpp ./mssc/dfState.cpp ./mssc/mssc.cpp ./TFA/Board.cpp ./TFA/Dfa.cpp ./TFA/Parser.cpp ./TFA/Pointer.cpp ./TFA/State.cpp ./TFA/Tfamain.cpp ./DFAtoRE/convert.cpp ./DFAtoRE/Files/nodes.cpp ./DFAtoRE/Files/parsers.cpp ./Product/Product.cpp ./Product/Automaton.cpp ./Product/FAParser.cpp ./Product/PA.cpp -o ./GUI/source/helper/simplify