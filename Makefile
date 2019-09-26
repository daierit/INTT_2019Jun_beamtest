CXX = g++ -std=c++11 -O3 -Wall
LDP = $(shell root-config --ld)
LIBP = $(shell root-config --libs)

all :
#make langaus
	make DAC_Scan_Fit

#langaus.o : langaus.C
#	$(CXX) -c -o $@ -g $< -MMD
#-include langaus.d

#langaus : langaus.o
#	$(LDP) -o $@ $+ $(LIBP)
	#$(LDP) -o $@ $+
#$(LDP) -o $@ $+ $(LIBP)

DAC_Scan_Fit.o : DAC_Scan_Fit.C
	$(CXX) -c -o $@ -g $< -MMD
-include DAC_Scan_Fit.d

DAC_Scan_Fit : DAC_Scan_Fit.o
	$(LDP) -o $@ $+ $(LIBP)

