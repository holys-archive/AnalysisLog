analysislog: AnalysisLog.o
	g++ -o $@ $^
AnalysisLog.o: AnalysisLog.cpp
	g++ -c $<
