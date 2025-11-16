CC = g++
flags = -W -std=c++20 -lpthread -O2 -g

headers = Exception/exception.h Utils/utils.h Trading/trading.h Simulation/simulation.h

exceptionFile = Exception/exceptionAIO.cpp
exceptionIncludes = $(headers) Exception/exception.cpp
exceptionObj = exception.o

utilsFile = Utils/utilsAIO.cpp 
utilsIncludes = $(headers) Utils/utilsTemplatesException.hpp Utils/utilsException.hpp Utils/tabExt.hpp Utils/tabExt2.hpp Utils/table.cpp
utilsObj = utils.o

mathFile = Math/math.cpp
mathIncludes = $(headers) Utils/utils.h  Math/math.ipp Math/math.h
mathObj = math.o

tradingFile = Trading/tradingAIO.cpp
tradingIncludes = $(headers) Trading/tradingException.hpp Trading/bougie.cpp Trading/timeFrame.cpp Trading/chart.cpp
tradingObj = trading.o

botsFile = Bots/smartDCA.cpp
botsIncludes = Trading/trading.h Bots/tradingBots.h Bots/tradingBots.tcc Bots/smartDCA.hpp
botsObj = bots.o


mainFile = work.cpp
objs = $(exceptionObj) $(utilsObj) $(mathObj) $(tradingObj) $(simulationObj) $(botsObj)
exe = test


all: $(exe) clean execution

$(exe):  $(objs) main.o
	@echo -n
	@echo edition des liens :
	@echo -n "\t"
	$(CC) -o $@ $(objs) main.o

$(tradingObj) : $(tradingFile)
	@echo -n
	@echo compilation de $(tradingFile):
	@echo -n "\t"
	$(CC) $(flags) -o $@ -c $(tradingFile)
$@ : $(tradingIncludes)

$(utilsObj) : $(utilsFile)
	@echo -n
	@echo compilation de $(utilsFile):
	@echo -n "\t"
	$(CC) $(flags) -o $@ -c $(utilsFile)
$@ : $(utilsInclude)

$(mathObj) : $(mathFile)
	@echo -n
	@echo compilation de $(mathFile):
	@echo -n "\t"
	$(CC) $(flags) -o $@ -c $(mathFile)
$@ : $(mathInclude)

$(botsObj) : $(botsFile)
	@echo -n
	@echo compilation de $(botsFile):
	@echo -n "\t"
	$(CC) $(flags) -o $@ -c $(botsFile)
$@ : $(botsInclude)

$(exceptionObj) : $(excetionFile)
	@echo -n
	@echo compilation de $(exceptionFile):
	@echo -n "\t"
	$(CC) $(flags) -o $@ -c $(exceptionFile)
$@ : $(exception)

main.o : $(mainFile)
	@echo -n
	@echo compilation de $(mainFile):
	@echo -n "\t"
	$(CC) $(flags) -o $@ -c $(mainFile)

execution:
	@echo -n
	@echo execution :
	@echo -n "\t"
	./$(exe)

clean:
	@echo -n
	@echo supperession des intermédiaires:
	@echo -n "\t"
	rm -f $(objs) main.o

.PHONY: all execution clean
