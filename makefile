objects=Complex.o Matrix.o Event.o ConfigLoader.o Global.o \
        GTT.o GTT_HighSpeed.o GTT_Urban.o IMTA.o \
        RRM.o RRM_ICC_DRA.o RRM_RR.o RRM_TDM_DRA.o \
        TMC.o TMC_B.o \
        WT.o WT_B.o \
        System.o main.o \

LTEV2X.out:${objects}
	g++ --std=gnu++11 -pthread -o LTEV2X.out ${objects} -lfftw3
Complex.o:Complex.cpp Complex.h Exception.h
	g++ --std=gnu++11 -o Complex.o -c Complex.cpp
Matrix.o:Matrix.cpp Matrix.h Function.h Complex.h Exception.h
	g++ --std=gnu++11 -o Matrix.o -c Matrix.cpp
Event.o:Event.cpp Event.h Exception.h Global.h Enumeration.h
	g++ --std=gnu++11 -o Event.o -c Event.cpp
ConfigLoader.o:ConfigLoader.cpp ConfigLoader.h Exception.h
	g++ --std=gnu++11 -o ConfigLoader.o -c ConfigLoader.cpp
Global.o:Global.cpp Global.h Function.h Enumeration.h
	g++ --std=gnu++11 -o Global.o -c Global.cpp
GTT.o:GTT.cpp GTT.h Function.h System.h
	g++ --std=gnu++11 -o GTT.o -c GTT.cpp
GTT_HighSpeed.o:GTT_HighSpeed.cpp GTT_HighSpeed.h Exception.h Function.h System.h
	g++ --std=gnu++11 -o GTT_HighSpeed.o -c GTT_HighSpeed.cpp
GTT_Urban.o:GTT_Urban.cpp GTT_Urban.h Exception.h Function.h System.h
	g++ --std=gnu++11 -o GTT_Urban.o -c GTT_Urban.cpp
IMTA.o:IMTA.cpp IMTA.h Function.h
	g++ --std=gnu++11 -o IMTA.o -c IMTA.cpp
RRM.o:RRM.cpp RRM.h Function.h System.h
	g++ --std=gnu++11 -o RRM.o -c RRM.cpp
RRM_ICC_DRA.o:RRM_ICC_DRA.cpp RRM_ICC_DRA.h Function.h System.h
	g++ --std=gnu++11 -pthread -o RRM_ICC_DRA.o -c RRM_ICC_DRA.cpp
RRM_RR.o:RRM_RR.cpp RRM_RR.h Function.h System.h
	g++ --std=gnu++11 -pthread -o RRM_RR.o -c RRM_RR.cpp
RRM_TDM_DRA.o:RRM_TDM_DRA.cpp RRM_TDM_DRA.h Exception.h Function.h System.h
	g++ --std=gnu++11 -pthread -o RRM_TDM_DRA.o -c RRM_TDM_DRA.cpp
TMC.o:TMC.cpp TMC.h Function.h System.h
	g++ --std=gnu++11 -o TMC.o -c TMC.cpp
TMC_B.o:TMC_B.cpp TMC_B.h Exception.h System.h
	g++ --std=gnu++11 -o TMC_B.o -c TMC_B.cpp
WT.o:WT.cpp WT.h Function.h System.h Matrix.h
	g++ --std=gnu++11 -o WT.o -c WT.cpp
WT_B.o:WT_B.cpp  WT_B.h System.h
	g++ --std=gnu++11 -o WT_B.o -c WT_B.cpp
System.o:System.cpp System.h Function.h
	g++ --std=gnu++11 -o System.o -c System.cpp
main.o:main.cpp
	g++ --std=gnu++11 -o main.o -c main.cpp
clean:
	-rm -f ${objects}


