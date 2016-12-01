objects=Complex.o ConfigLoader.o Event.o GTT.o GTT_HighSpeed.o GTT_Urban.o IMTA.o \
        main.o Matrix.o RRM.o RRM_ICC_DRA.o RRM_RR.o RRM_TDM_DRA.o System.o \
        TMC.o WT.o

LTEV2X.out:${objects}
	g++ --std=gnu++11 -pthread -o LTEV2X.out ${objects} -lfftw3
Complex.o:Complex.cpp Complex.h
	g++ --std=gnu++11 -o Complex.o -c Complex.cpp

ConfigLoader.o:ConfigLoader.cpp ConfigLoader.h
	g++ --std=gnu++11 -o ConfigLoader.o -c ConfigLoader.cpp

Event.o:Event.cpp Event.h TMC.h \
	Enumeration.h 
	g++ --std=gnu++11 -o Event.o -c Event.cpp

GTT.o:GTT.cpp System.h GTT.h IMTA.h Function.h \
	Config.h Event.h \
	Enumeration.h
	g++ --std=gnu++11 -o GTT.o -c GTT.cpp

GTT_HighSpeed.o:GTT_HighSpeed.cpp System.h GTT_HighSpeed.h IMTA.h Function.h \
	Config.h Event.h GTT.h \
	Enumeration.h
	g++ --std=gnu++11 -o GTT_HighSpeed.o -c GTT_HighSpeed.cpp

GTT_Urban.o:GTT_Urban.cpp System.h GTT_Urban.h IMTA.h Function.h \
	Config.h Event.h GTT.h \
	Enumeration.h
	g++ --std=gnu++11 -o GTT_Urban.o -c GTT_Urban.cpp

IMTA.o:IMTA.cpp IMTA.h Function.h \
	Config.h
	g++ --std=gnu++11 -o IMTA.o -c IMTA.cpp

main.o:main.cpp Enumeration.h System.h Matrix.h Function.h \
	Config.h Event.h Complex.h \
	Enumeration.h
	g++ --std=gnu++11 -o main.o -c main.cpp

Matrix.o:Matrix.cpp Matrix.h Function.h \
	Complex.h
	g++ --std=gnu++11 -o Matrix.o -c Matrix.cpp

RRM.o:RRM.cpp System.h RRM.h VUE.h RSU.h Function.h \
	Config.h Event.h Enumeration.h
	g++ --std=gnu++11 -o RRM.o -c RRM.cpp

RRM_ICC_DRA.o:RRM_ICC_DRA.cpp System.h GTT.h RRM_ICC_DRA.h WT.h VUE.h RSU.h Function.h \
	Config.h Event.h RRM.h Enumeration.h
	g++ --std=gnu++11 -pthread -o RRM_ICC_DRA.o -c RRM_ICC_DRA.cpp

RRM_RR.o:RRM_RR.cpp System.h GTT.h WT.h RRM_RR.h VUE.h RSU.h Function.h \
	Config.h Event.h Enumeration.h RRM.h
	g++ --std=gnu++11 -pthread -o RRM_RR.o -c RRM_RR.cpp

RRM_TDM_DRA.o:RRM_TDM_DRA.cpp System.h GTT.h RRM_TDM_DRA.h WT.h VUE.h RSU.h Function.h \
	Config.h Event.h RRM.h Enumeration.h
	g++ --std=gnu++11 -pthread -o RRM_TDM_DRA.o -c RRM_TDM_DRA.cpp

System.o:System.cpp System.h GTT.h GTT_HighSpeed.h GTT_Urban.h RRM.h RRM_ICC_DRA.h RRM_TDM_DRA.h RRM_RR.h \
	TMC.h WT.h VUE.h RSU.h eNB.h Road.h Config.h Function.h ConfigLoader.h  
	g++ --std=gnu++11 -o System.o -c System.cpp

TMC.o:TMC.cpp TMC.h System.h Function.h \
	Config.h Event.h Enumeration.h
	g++ --std=gnu++11 -o TMC.o -c TMC.cpp

WT.o:WT.cpp WT.h System.h Function.h \
	Config.h Event.h Enumeration.h
	g++ --std=gnu++11 -o WT.o -c WT.cpp

clean:
	-rm -f ${objects}


